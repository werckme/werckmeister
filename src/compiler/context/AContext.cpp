#include "AContext.h"
#include <compiler/error.hpp>
#include <fm/werckmeister.hpp>
#include <algorithm>
#include <fm/common.hpp>
#include <compiler/spielanweisung/ASpielanweisung.h>
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/modification/AModification.h>
#include <fm/literals.hpp>
#include <fm/config/configServer.h>
#include <sheet/objects/Track.h>
#include <fm/tools.h>
#include <sstream>
#include <fm/config/configServer.h>

namespace sheet {

	namespace compiler {
		using namespace fm;
		const double AContext::PitchbendMiddle = 0.5;
		const Ticks AContext::TickTolerance = 0.5;
		
		AContext::AContext(fm::IDefinitionsServerPtr definitionsServer)
			: definitionsServer_(definitionsServer)
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
			if (meta->spielanweisungOnce) {
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
			if (it == voiceMetaDataMap_.end()) {
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
			if (it == trackMetaDataMap_.end()) {
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

		fm::Ticks AContext::currentPosition() const
		{
			auto voiceMeta = voiceMetaData();
			if (!voiceMeta) {
				return 0;
			}
			return voiceMeta->position;
		}

		fm::Ticks AContext::maxPosition() const
		{
			fm::Ticks position = 0;
			for (const auto &keyValue : voiceMetaDataMap_) {
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
			if (velOverride != instrument->velocityOverride.end()) {
				return velOverride->second / 100.;
			}
			double expr = static_cast<double>(expression);
			return  expr / 10.;
		}

		void AContext::renderPitch(const PitchDef &rawPitch, fm::Ticks duration, double velocity, bool tying)
		{
			using namespace fm;
			PitchDef pitch = definitionsServer_->resolvePitch(rawPitch);
			auto meta = voiceMetaData();
			if (tying) {
				auto alreadyTying = meta->waitForTieBuffer.find(pitch) != meta->waitForTieBuffer.end();
				if (!alreadyTying) {
					meta->waitForTieBuffer.insert({ pitch, duration });
					startEvent(pitch, meta->position, velocity);
				}
				return;
			}
			if (meta->pendingTie()) {
				auto it = meta->waitForTieBuffer.find(pitch);
				if (it != meta->waitForTieBuffer.end()) {
					stopEvent(pitch, meta->position + duration);
					meta->waitForTieBuffer.erase(it);
					return;
				}
			}
			renderPitch(pitch, meta->position, velocity, duration);
		}

		void AContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity)
		{
			auto meta = voiceMetaData();
			meta->startedEvents.insert(pitch);
		}

		void AContext::stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			auto meta = voiceMetaData();
			meta->startedEvents.erase(pitch);
		}

		fm::Ticks AContext::barPos() const
		{
			auto meta = voiceMetaData();
			return meta->barPosition;
		}

		void AContext::seek(fm::Ticks duration)
		{
			auto meta = voiceMetaData();
			meta->position += duration;
			meta->barPosition += duration;
		}

		void AContext::warn(const std::string &msg)
		{
			if (_warningHandler) {
				_warningHandler(msg);
			}
		}

		void AContext::newBar()
		{
			auto meta = voiceMetaData();
			if (!fm::compareTolerant(meta->barPosition, meta->barLength, fm::Ticks(TickTolerance))) {
				auto errorInQuaters = -(meta->barLength - meta->barPosition) / fm::PPQ;
				seek(-(meta->barPosition - meta->barLength));
				std::stringstream ss;
				std::string toLong = errorInQuaters > 0 ? "too long" : "too short";
				ss << "bar is " << std::abs(errorInQuaters) << " quarter(s) " << toLong;
				warn(ss.str());
			}
			meta->barPosition = 0;
			++(meta->barCount);
		}

		void AContext::rest(fm::Ticks duration)
		{
			auto meta = voiceMetaData();
			seek(duration);
		}
		void AContext::setVolume(double volume)
		{
			auto meta = voiceMetaData();
			meta->volume = std::max(std::min(volume, 100.0), 0.0);
		}

		void AContext::setPan(double val)
		{
			auto meta = voiceMetaData();
			meta->pan = std::max(std::min(val, 100.0), 0.0);
		}		

		void AContext::setExpression(fm::Expression expr)
		{
			if (expr == fm::expression::Default) {
				return;
			}
			auto meta = voiceMetaData();
			meta->expression = expr;
		}

		void AContext::setExpressionPlayedOnce(fm::Expression expr)
		{
			if (expr == fm::expression::Default) {
				return;
			}
			auto meta = voiceMetaData();
			meta->expressionPlayedOnce = expr;
		}

		void AContext::setSignature(int upper, int lower)
		{
			using namespace fm;
			auto meta = voiceMetaData();
			meta->signatureNumerator = upper;
			meta->signatureDenominator = lower;
			meta->barLength = (1.0_N1 / (fm::Ticks)lower) * (fm::Ticks)upper;
		}

		void AContext::setChordTrackTarget()
		{
			if (chordTrack_ == INVALID_TRACK_ID) {
				chordTrack_ = createTrack();
				chordVoice_ = createVoice();
			}
			setTarget(chordTrack_, chordVoice_);
		}

		TimeInfo AContext::getTimeInfo() const
		{
			TimeInfo result;
			auto meta = voiceMetaData();
			result.quarterPosition = meta->position / (double)fm::PPQ;
			result.signatureNumerator = meta->signatureNumerator;
			result.sinatureDenominator = meta->signatureDenominator;
			return result;
		}


		const AContext::SheetTemplates & AContext::currentSheetTemplates()
		{
			if (currentSheetTemplates_.empty()) {
				auto defaultTemplate = definitionsServer_->getSheetTemplate(definitionsServer_->defaultSheetTemplateName());
				currentSheetTemplates_.push_back(defaultTemplate);
			}
			return currentSheetTemplates_;
		}
		void AContext::currentSheetTemplate(const SheetTemplates &sheetTemplate)
		{
			currentSheetTemplates_ = sheetTemplate;
		}
		VoicingStrategyPtr AContext::currentVoicingStrategy()
		{
			auto meta = voiceMetaData();
			if (!defaultVoiceStrategy_) {
				defaultVoiceStrategy_ = fm::getWerckmeister().getDefaultVoicingStrategy();
			}
			if (meta->voicingStrategy) { // first voice setup
				return meta->voicingStrategy;
			}
			auto currentInstrument = currentInstrumentDef();
			if (currentInstrument && currentInstrument->voicingStrategy) { // then instrument config
				return currentInstrument->voicingStrategy;
			}
			return defaultVoiceStrategy_;
		}

		void AContext::clear()
		{
			masterTempo_ = fm::DefaultTempo;
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

		void AContext::setInstrument(const fm::String& uname)
		{
		}
	}
}