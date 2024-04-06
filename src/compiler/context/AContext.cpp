#include "AContext.h"
#include <compiler/error.hpp>
#include <com/werckmeister.hpp>
#include <algorithm>
#include <com/common.hpp>
#include <compiler/spielanweisung/ASpielanweisung.h>
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/modification/AModification.h>
#include <com/literals.hpp>
#include <com/config/configServer.h>
#include <documentModel/objects/Track.h>
#include <com/tools.h>
#include <sstream>
#include <com/config/configServer.h>
#include <com/OnLeavingScope.hpp>

namespace compiler
{
	using namespace com;
	const double AContext::PitchbendMiddle = 0.5;
	const Ticks AContext::TickTolerance = 0.5;
	const documentModel::Event fallbackChordEvent = [](){
		documentModel::Event ev;
		ev.type = documentModel::Event::Chord;
		ev.stringValue = "C";
		ev.tags.insert("fallback");
		return ev;
	}();

	AContext::AContext(compiler::IDefinitionsServerPtr definitionsServer, ICompilerVisitorPtr compilerVisitor)
		: definitionsServer_(definitionsServer), _compilerVisitor(compilerVisitor)
	{
	}

	AContext::TrackId AContext::createMasterTrack()
	{
		this->masterTrackId_ = createTrack();
		return this->masterTrackId_;
	}

	AModificationPtr AContext::spielanweisung()
	{
		auto meta = voiceMetaData();
		if (meta->spielanweisungOnce)
		{
			auto tmp = meta->spielanweisungOnce;
			meta->spielanweisungOnce.reset();
			return tmp;
		}
		return meta->spielanweisung;
	}

	AContext::TrackId AContext::track() const
	{
		return trackId_;
	}
	AContext::VoiceId AContext::voice() const
	{
		return voiceId_;
	}

	void AContext::setTrack(TrackId trackId)
	{
		this->voiceId_ = INVALID_VOICE_ID;
		this->trackId_ = trackId;
	}

	void AContext::setVoice(VoiceId voice)
	{
		this->voiceId_ = voice;
	}

	AContext::TrackId AContext::createTrack()
	{
		auto tid = createTrackImpl();
		auto meta = createTrackMetaData();
		trackMetaDataMap_[tid] = meta;
		return tid;
	}
	AContext::VoiceId AContext::createVoice()
	{
		VoiceId vid = createVoiceImpl();
		auto meta = createVoiceMetaData();
		voiceMetaDataMap_[vid] = meta;
		return vid;
	}

	AContext::VoiceMetaDataPtr AContext::voiceMetaData(VoiceId voiceid) const
	{
		auto it = voiceMetaDataMap_.find(voiceid);
		if (it == voiceMetaDataMap_.end())
		{
			return nullptr;
		}
		return it->second;
	}

	AContext::VoiceMetaDataPtr AContext::voiceMetaData() const
	{
		return voiceMetaData(voice());
	}

	AContext::TrackMetaDataPtr AContext::trackMetaData(TrackId trackid) const
	{
		auto it = trackMetaDataMap_.find(trackid);
		if (it == trackMetaDataMap_.end())
		{
			return nullptr;
		}
		return it->second;
	}

	AContext::TrackMetaDataPtr AContext::trackMetaData() const
	{
		return trackMetaData(track());
	}

	void AContext::throwContextException(const std::string &msg)
	{
		auto meta = voiceMetaData();
		FM_THROW(Exception, msg + " at voice " + std::to_string(voice()) + " bar: " + std::to_string(meta->position / meta->barLength));
	}

	com::Ticks AContext::currentPosition() const
	{
		auto voiceMeta = voiceMetaData();
		if (!voiceMeta)
		{
			return 0;
		}
		return voiceMeta->position;
	}

	com::Ticks AContext::maxPosition() const
	{
		com::Ticks position = 0;
		for (const auto &keyValue : voiceMetaDataMap_)
		{
			const auto &meta = keyValue.second;
			position = std::max(position, meta->position);
		}
		return position;
	}

	double AContext::velocity()
	{
		auto instrument = currentInstrumentDef();
		auto expression = voiceMetaData()->expression;
		auto velOverride = instrument->velocityOverride.find(expression);
		if (velOverride != instrument->velocityOverride.end())
		{
			return velOverride->second / 100.;
		}
		double expr = static_cast<double>(expression);
		return expr / 10.;
	}

	void AContext::stopAllPendingTies()
	{
		auto meta = voiceMetaData();
		for (const auto &tie : meta->waitForTieBuffer)
		{
			stopEvent(tie.first, meta->position);
		}
		meta->waitForTieBuffer.clear();
	}
	void AContext::renderPitch(const documentModel::PitchDef &rawPitch, com::Ticks duration, double velocity, bool tying)
	{
		using namespace com;
		documentModel::PitchDef pitch = definitionsServer_->resolvePitch(rawPitch);
		_compilerVisitor->beginRenderPitch(pitch);
		com::OnLeavingScope onLeavingScope([this]{ _compilerVisitor->endRenderPitch(); });
		auto meta = voiceMetaData();
		if (tying)
		{
			auto alreadyTying = meta->waitForTieBuffer.find(pitch) != meta->waitForTieBuffer.end();
			if (!alreadyTying)
			{
				meta->waitForTieBuffer.insert({pitch, duration});
				startEvent(pitch, meta->position, velocity);
			}
			return;
		}
		if (meta->pendingTie())
		{
			auto it = meta->waitForTieBuffer.find(pitch);
			if (it != meta->waitForTieBuffer.end())
			{
				stopEvent(pitch, meta->position + duration);
				meta->waitForTieBuffer.erase(it);
				return;
			}
		}
		renderPitch(pitch, meta->position, velocity, duration);
	}

	void AContext::startEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition, double velocity)
	{
		auto meta = voiceMetaData();
		meta->startedEvents.insert(pitch);
	}

	void AContext::stopEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition)
	{
		auto meta = voiceMetaData();
		meta->startedEvents.erase(pitch);
	}

	com::Ticks AContext::barPos() const
	{
		auto meta = voiceMetaData();
		return meta->barPosition;
	}

	void AContext::seek(com::Ticks duration)
	{
		auto meta = voiceMetaData();
		meta->position += duration;
		meta->barPosition += duration;
	}

	void AContext::warn(const std::string &msg)
	{
		if (_warningHandler)
		{
			_warningHandler(msg);
		}
	}

	void AContext::newBar()
	{
		auto meta = voiceMetaData();
		if (!com::compareTolerant(meta->barPosition, meta->barLength, com::Ticks(TickTolerance)))
		{
			auto errorInQuaters = -(meta->barLength - meta->barPosition) / com::PPQ;
			seek(-(meta->barPosition - meta->barLength));
			std::stringstream ss;
			std::string toLong = errorInQuaters > 0 ? "too long" : "too short";
			ss << "bar is " << std::abs(errorInQuaters) << " quarter(s) " << toLong;
			warn(ss.str());
		}
		meta->barPosition = 0;
		++(meta->barCount);
	}

	void AContext::rest(com::Ticks duration)
	{
		auto meta = voiceMetaData();
		seek(duration);
	}
	void AContext::setVolume(double volume, com::Ticks relativePosition)
	{
		auto meta = voiceMetaData();
		meta->volume = std::max(std::min(volume, 100.0), 0.0);
	}

	void AContext::setPan(double val)
	{
		auto meta = voiceMetaData();
		meta->pan = std::max(std::min(val, 100.0), 0.0);
	}

	void AContext::setExpression(com::Expression expr)
	{
		if (expr == com::expression::Default)
		{
			return;
		}
		auto meta = voiceMetaData();
		meta->expression = expr;
	}

	void AContext::setExpressionPlayedOnce(com::Expression expr)
	{
		if (expr == com::expression::Default)
		{
			return;
		}
		auto meta = voiceMetaData();
		meta->expressionPlayedOnce = expr;
	}

	void AContext::setSignature(int upper, int lower)
	{
		using namespace com;
		auto meta = voiceMetaData();
		meta->signatureNumerator = upper;
		meta->signatureDenominator = lower;
		meta->barLength = (1.0_N1 / (com::Ticks)lower) * (com::Ticks)upper;
	}

	void AContext::setChordTrackTarget()
	{
		if (chordTrack_ != INVALID_TRACK_ID)
		{
			setTarget(chordTrack_, chordVoice_);
			return;
		}
		chordTrack_ = createTrack();
		chordVoice_ = createVoice();
		setTarget(chordTrack_, chordVoice_);
		auto trackMeta = trackMetaData();
		trackMeta->uname = "Accomp";
	}

	TimeInfo AContext::getTimeInfo() const
	{
		TimeInfo result;
		auto meta = voiceMetaData();
		result.quarterPosition = meta->position / (double)com::PPQ;
		result.signatureNumerator = meta->signatureNumerator;
		result.sinatureDenominator = meta->signatureDenominator;
		return result;
	}

	const AContext::SheetTemplates &AContext::currentSheetTemplates()
	{
		if (currentSheetTemplates_.empty())
		{
			auto defaultTemplate = definitionsServer_->getSheetTemplate(definitionsServer_->defaultSheetTemplateName());
			currentSheetTemplates_.push_back(defaultTemplate);
		}
		return currentSheetTemplates_;
	}
	void AContext::currentSheetTemplate(const SheetTemplates &sheetTemplate)
	{
		currentSheetTemplates_ = sheetTemplate;
	}
	const VoicingStrategies& AContext::voicingStrategies()
	{
		auto meta = voiceMetaData();
		if (!defaultVoiceStrategy_)
		{
			defaultVoiceStrategy_ = com::getWerckmeister().getDefaultVoicingStrategy();
		}
		if (!meta->voicingStrategies.empty())
		{ // first voice setup
			return meta->voicingStrategies;
		}
		auto currentInstrument = currentInstrumentDef();
		if (currentInstrument && !currentInstrument->voicingStrategies.empty())
		{ // then instrument config
			return currentInstrument->voicingStrategies;
		}
		static VoicingStrategies defaultVoiceStrategies({defaultVoiceStrategy_});
		return defaultVoiceStrategies;
	}

	const documentModel::Event& AContext::currentChordEvent() const
	{
		auto meta = voiceMetaData();
		if (!meta)
		{
			FM_THROW(Exception, "meta data = null");
		}
		if (meta->chordEvent.type != documentModel::Event::Chord)
		{
			return fallbackChordEvent;
		}
		return meta->chordEvent;
	}

	void AContext::currentChordEvent(const documentModel::Event& chordEvent)
	{
		if (chordEvent.type != documentModel::Event::Chord)
		{
			FM_THROW(Exception, "expecting a chord event but got: " + chordEvent.toString());
		}
		auto meta = voiceMetaData();
		if (!meta)
		{
			FM_THROW(Exception, "meta data = null");
		}
		meta->chordEvent = chordEvent;
	}

	void AContext::clear()
	{
		masterTempo_ = com::DefaultTempo;
		defaultVoiceStrategy_ = nullptr;
		currentSheetTemplates_.clear();
		TrackId trackId_ = INVALID_TRACK_ID;
		chordTrack_ = INVALID_TRACK_ID;
		masterTrackId_ = INVALID_TRACK_ID;
		VoiceId voiceId_ = INVALID_VOICE_ID;
		chordVoice_ = INVALID_VOICE_ID;
		voiceMetaDataMap_.clear();
		trackMetaDataMap_.clear();
	}

	void AContext::setInstrument(const com::String &uname)
	{
	}

	void AContext::addTag(const com::String &tag)
	{
		auto metaData = voiceMetaData();
		metaData->tags.insert(tag);
	}
	void AContext::removeTag(const com::String &tag)
	{
		auto metaData = voiceMetaData();
		metaData->tags.erase(tag);
	}
	const VoiceMetaData::Tags & AContext::getTags() const
	{
		auto metaData = voiceMetaData();
		return metaData->tags;
	}
}