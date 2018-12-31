#include "AContext.h"
#include <exception>
#include <exception>
#include <fm/werckmeister.hpp>
#include <algorithm>
#include <fm/common.hpp>
#include "spielanweisung/ASpielanweisung.h"
#include "modification/AModification.h"
#include <fm/literals.hpp>

namespace {
	const fm::Ticks TickTolerance = 0.5; // rounding errors e.g. for triplets
}

namespace sheet {

	namespace compiler {
		using namespace fm;
		const Ticks AContext::DefaultDuration = 1.0_N4;
		const Ticks AContext::DefaultBarLength = 4 * 1.0_N4;
		const double AContext::PitchbendMiddle = 0.5;
		namespace {
			template<int EventType>
			bool renderEvent(AContext * ctx, const Event *ev)
			{
				return false;
			}

			template<>
			bool renderEvent<Event::Note>(AContext * ctx, const Event *ev)
			{
				ctx->addEvent(ev->pitches, ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::Degree>(AContext * ctx, const Event *ev)
			{
				auto chordDef = ctx->currentChordDef();
				auto chord = ctx->currentChord();
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(*chord, *chordDef, ev->pitches, VoicingStrategy::TimeInfo());

				ctx->addEvent(pitches, ev->duration);

				return true;
			}

			template<>
			bool renderEvent<Event::TiedDegree>(AContext * ctx, const Event *ev)
			{
				auto chordDef = ctx->currentChordDef();
				auto chord = ctx->currentChord();
				auto voicingStratgy = ctx->currentVoicingStrategy();
				auto pitches = voicingStratgy->get(*chord, *chordDef, ev->pitches, VoicingStrategy::TimeInfo());

				ctx->addEvent(pitches, ev->duration, true);

				return true;
			}

			template<>
			bool renderEvent<Event::TiedNote>(AContext * ctx, const Event *ev)
			{

				ctx->addEvent(ev->pitches, ev->duration, true);
				return true;
			}

			template<>
			bool renderEvent<Event::EOB>(AContext * ctx, const Event *ev)
			{
				ctx->newBar();
				return true;
			}

			template<>
			bool renderEvent<Event::Rest>(AContext * ctx, const Event *ev)
			{
				ctx->rest(ev->duration);
				return true;
			}

			template<>
			bool renderEvent<Event::Chord>(AContext * ctx, const Event *ev)
			{
				auto chordEv = static_cast<const ChordEvent*>(ev);
				ctx->setChord(*chordEv);
				ctx->seek(ev->duration);
				return true;
			}
			template<>
			bool renderEvent<Event::Meta>(AContext * ctx, const Event *ev)
			{
				ctx->setMeta(*ev);
				return true;
			}
			//////////////////////////////////////////////////
			template <int EventId>
			bool renderEventUnrolled(AContext * ctx, const Event *ev)
			{
				if (ev->type == EventId) {
					return renderEvent<EventId>(ctx, ev);
				}
				return renderEventUnrolled<EventId + 1>(ctx, ev);
			}
			template <>
			bool renderEventUnrolled<Event::NumEvents>(AContext * ctx, const Event *ev)
			{
				return false;
			}

			void _addEvent(AContext * ctx, const Event *ev)
			{
				renderEventUnrolled<0>(ctx, ev);
			}
		}


		AContext::AContext() 
			: expressionMap_({
				{FM_STRING("ppppp"), fm::expression::PPPPP},
				{ FM_STRING("pppp"), fm::expression::PPPP },
				{ FM_STRING("ppp"), fm::expression::PPP },
				{ FM_STRING("pp"), fm::expression::PP },
				{ FM_STRING("p"), fm::expression::P },
				{ FM_STRING("f"), fm::expression::F },
				{ FM_STRING("ff"), fm::expression::FF },
				{ FM_STRING("fff"), fm::expression::FFF },
				{ FM_STRING("ffff"), fm::expression::FFFF },
				{ FM_STRING("fffff"), fm::expression::FFFFF }
			})
		{
		}

		ASpielanweisungPtr AContext::spielanweisung()
		{
			auto meta = voiceMetaData(voice());
			if (!defaultSpielanweisung_) {
				defaultSpielanweisung_ = fm::getWerckmeister().getDefaultSpielanweisung();
			}
			if (meta->spielanweisungOnce) {
				auto tmp = meta->spielanweisungOnce;
				meta->spielanweisungOnce.reset();
				return tmp;
			}
			if (!meta->spielanweisung) {
				return defaultSpielanweisung_;
			}
			return meta->spielanweisung;
		}

		fm::Expression AContext::getExpression(const fm::String &str) const
		{
			auto it = expressionMap_.find(str);
			if (it == expressionMap_.end()) {
				return fm::expression::Default;
			}
			return it->second;
		}

		AContext::IStyleDefServerPtr AContext::styleDefServer() const
		{
			if (!styleDefServer_) {
				throw std::runtime_error("no styledef server set");
			}
			return styleDefServer_;
		}
		void AContext::styleDefServer(IStyleDefServerPtr server)
		{
			styleDefServer_ = server;
		}

		AContext::TrackId AContext::track() const
		{
			if (trackId_ == INVALID_TRACK_ID) {
				throw std::runtime_error("no track set");
			}
			return trackId_;
		}
		AContext::VoiceId AContext::voice() const
		{
			if (voiceId_ == INVALID_VOICE_ID) {
				throw std::runtime_error("no voice set");
			}
			return voiceId_;
		}

		void AContext::setTrack(TrackId trackId)
		{
			this->trackId_ = trackId;
		}

		void AContext::setVoice(VoiceId voice)
		{
			this->voiceId_ = voice;
		}

		AContext::TrackId AContext::createTrack()
		{
			auto tid = createTrackImpl();
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
				throw std::runtime_error("no meta data found for voiceid: " + std::to_string(voiceid));
			}
			return it->second;
		}

		void AContext::throwContextException(const std::string &msg)
		{
			auto meta = voiceMetaData(voice());
			throw std::runtime_error(msg + " at voice " + std::to_string(voice()) + " bar: " + std::to_string(meta->position / meta->barLength));
		}

		PitchDef AContext::resolvePitch(const PitchDef &pitch) const
		{
			if (pitch.alias.empty()) {
				return pitch;
			}
			const PitchDef *result = styleDefServer()->getAlias(pitch.alias);
			if (result == nullptr) {
				throw std::runtime_error("could not resolve alias: " + fm::to_string(pitch.alias));
			}
			return *result;
		}

		void AContext::addEvent(const PitchDef &rawPitch, fm::Ticks duration, bool tying)
		{
			using namespace fm;
			PitchDef pitch = resolvePitch(rawPitch);
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			if (tying) {
				meta->waitForTieBuffer.insert({ pitch, meta->duration });
				startEvent(pitch, meta->position);
			}
			else if (meta->pendingTie()) {
				auto it = meta->waitForTieBuffer.find(pitch);
				if (it == meta->waitForTieBuffer.end()) {
					stopTying();
					return;
				}
				stopEvent(pitch, meta->position + meta->duration);
				meta->waitForTieBuffer.erase(it);
				return;
			}
			else {
				addEvent(pitch, meta->position, meta->duration);
			}
		}

		void AContext::stopTying()
		{
			auto meta = voiceMetaData(voice());
			for(auto tied : meta->startedEvents){
				stopEvent(tied, meta->position + meta->duration);
			}
		}

		void AContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			auto meta = voiceMetaData(voice());
			meta->startedEvents.insert(pitch);
		}

		void AContext::stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
		{
			auto meta = voiceMetaData(voice());
			meta->startedEvents.erase(pitch);
		}

		fm::Ticks AContext::barPos() const
		{
			auto meta = voiceMetaData(voice());
			return meta->barPosition;
		}

		void AContext::addEvent(const Event &ev)
		{
			auto meta = voiceMetaData(voice());
			++(meta->eventCount);
			_addEvent(this, &ev);
		}

		void AContext::addEvent(const Event::Pitches &pitches, fm::Ticks duration, bool tying)
		{
			auto meta = voiceMetaData(voice());
			auto tmpExpression = meta->expression;

			if (meta->singleExpression != fm::expression::Default) {
				tmpExpression = meta->expression;
				meta->expression = meta->singleExpression;
				meta->singleExpression = fm::expression::Default;
			}
			for (auto mod : meta->modifications) {
				mod->addModificationEvents(this, meta->position, duration);
			}
			for (auto mod : meta->modificationsOnce) {
				mod->addModificationEvents(this, meta->position, duration);
			}			
			meta->modificationsOnce.clear();
			auto sanweis = spielanweisung();
			sanweis->addEvent(this, pitches, duration, tying);
			meta->expression = tmpExpression;
		}

		void AContext::seek(fm::Ticks duration)
		{
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			meta->position += meta->duration;
			meta->barPosition += meta->duration;
		}

		void AContext::warn(const std::string &msg)
		{
			auto meta = voiceMetaData(voice());
			std::string warning(msg + " at voice " + std::to_string(voice()) + " bar: " + std::to_string(meta->position / meta->barLength));
			warnings.push_back(warning);
		}

		void AContext::newBar()
		{
			auto meta = voiceMetaData(voice());
			if (meta->isUpbeat) {
				meta->isUpbeat = false;
				meta->eventOffset = meta->eventCount;
			}
			else if (!fm::compareTolerant(meta->barPosition, meta->barLength, fm::Ticks(TickTolerance))) {
				warn("bar check error (" + std::to_string(fm::absDifference(meta->barPosition, meta->barLength)) + ")");
			}
			meta->barPosition = 0;
			++(meta->barCount);
		}

		void AContext::rest(fm::Ticks duration)
		{
			seek(duration);
		}

		void AContext::setMeta(const Event &metaEvent)
		{
			if (metaEvent.metaCommand.empty()) {
				throwContextException("invalid meta command ");
			}
			if (metaEvent.metaCommand == SHEET_META__SET_UNAME) {
				metaSetUname(getArgument<fm::String>(metaEvent, 0));
			}
			if (metaEvent.metaCommand == SHEET_META__SET_STYLE) {
				metaSetStyle(getArgument<fm::String>(metaEvent, 0), getArgument<fm::String>(metaEvent, 1));
			}
			if (metaEvent.metaCommand == SHEET_META__SET_EXPRESSION) {
				metaSetExpression(getArgument<fm::String>(metaEvent, 0));
			}
			if (metaEvent.metaCommand == SHEET_META__SET_SINGLE_EXPRESSION) {
				metaSetSingleExpression(getArgument<fm::String>(metaEvent, 0));
			}
			if (metaEvent.metaCommand == SHEET_META__SET_TEMPO) { 
				metaSetTempo(getArgument<fm::BPM>(metaEvent, 0));
			}
			if (metaEvent.metaCommand == SHEET_META__SET_UPBEAT) { 
				metaSetUpbeat(metaEvent);
			}
			if (metaEvent.metaCommand == SHEET_META__SET_VOICING_STRATEGY) {
				metaSetVoicingStrategy(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}
			if (metaEvent.metaCommand == SHEET_META__SET_SPIELANWEISUNG) {
				metaSetSpielanweisung(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}	
			if (metaEvent.metaCommand == SHEET_META__SET_SPIELANWEISUNG_ONCE) {
				metaSetSpielanweisungOnce(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}	
			if (metaEvent.metaCommand == SHEET_META__SET_MOD) {
				metaSetModification(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}	
			if (metaEvent.metaCommand == SHEET_META__SET_MOD_ONCE) {
				metaSetModificationOnce(getArgument<fm::String>(metaEvent, 0), metaEvent.metaArgs);
			}
			if (metaEvent.metaCommand == SHEET_META__SET_SIGNATURE) {
				metaSetSignature(getArgument<int>(metaEvent, 0), getArgument<int>(metaEvent, 1));
			}															
		}

		void AContext::metaSetVoicingStrategy(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData(voice());
			meta->voicingStrategy = wm.getVoicingStrategy(name);
			meta->voicingStrategy->setArguments(args);
		}

		void AContext::metaSetSpielanweisung(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData(voice());
			meta->spielanweisung = wm.getSpielanweisung(name);
			meta->spielanweisung->setArguments(args);
		}

		void AContext::metaSetSpielanweisungOnce(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData(voice());
			meta->spielanweisungOnce = wm.getSpielanweisung(name);
			meta->spielanweisungOnce->setArguments(args);
		}

		void AContext::metaSetModification(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData(voice());
			auto mod = wm.getModification(name);
			meta->modifications.push_back(mod);
			mod->setArguments(args);
		}

		void AContext::metaSetModificationOnce(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData(voice());
			auto mod = wm.getModification(name);
			meta->modificationsOnce.push_back(mod);
			mod->setArguments(args);
		}

		void AContext::metaSetUname(const fm::String &uname)
		{
			auto meta = voiceMetaData(voice());
			meta->uname = uname;
		}

		void AContext::metaSetStyle(const fm::String &file, const fm::String &section)
		{
			auto style = styleDefServer_->getStyle(file, section);
			if (!style) {
				throw std::runtime_error("style not found: " + fm::to_string(file) + " " + fm::to_string(section));
			}
			switchStyle(currentStyleDef_, style);
		}

		void AContext::metaSetExpression(const fm::String &value)
		{
			auto meta = voiceMetaData(voice());
			auto expr = getExpression(value);
			if (expr == fm::expression::Default) {
				return;
			}
			meta->expression = expr;
		}

		void AContext::metaSetSingleExpression(const fm::String &value)
		{
			auto meta = voiceMetaData(voice());
			auto expr = getExpression(value);
			if (expr == fm::expression::Default) {
				return;
			}
			meta->singleExpression = expr;
		}

		void AContext::metaSetUpbeat(const Event &event) 
		{
			auto meta = voiceMetaData(voice());
			if (meta->position > 0) {
				throwContextException("upbeat only allowed on begin of track");
			}
			meta->isUpbeat = true;
		}

		void AContext::metaSetSignature(int upper, int lower)
		{
			using namespace fm;
			auto meta = voiceMetaData(voice());
			meta->barLength = (1.0_N1 / (fm::Ticks)lower) * (fm::Ticks)upper;
		}

		void AContext::switchStyle(IStyleDefServer::ConstStyleValueType current, IStyleDefServer::ConstStyleValueType next)
		{
			if (next == nullptr || current == next) {
				return;
			}
			// set position for new track
			auto chordMeta = voiceMetaData(chordVoice_);
			for (const auto &track : *next) {
				for (const auto &voice : track.voices)
				{
					setTarget(track, voice);
					auto meta = voiceMetaData(this->voice());
					meta->position = chordMeta->position;
				}
			}
			currentStyleDef_ = next;
		}

		void AContext::setChordTrackTarget()
		{
			if (chordTrack_ == INVALID_TRACK_ID) {
				chordTrack_ = createTrack();
				chordVoice_ = createVoice();
			}
			setTarget(chordTrack_, chordVoice_);
		}

		/////////////////////////////////////////////////////////////////////////////
		// Stylerendering
		IStyleDefServer::ConstChordValueType AContext::currentChordDef()
		{
			if (!currentChordDef_) {
				currentChordDef_ = styleDefServer()->getChord(FM_STRING("?"));
			}
			return currentChordDef_;
		}
		IStyleDefServer::ConstStyleValueType AContext::currentStyle()
		{
			if (!currentStyleDef_) {
				currentStyleDef_ = styleDefServer()->getStyle(FM_STRING("?"));
			}
			return currentStyleDef_;
		}
		VoicingStrategyPtr AContext::currentVoicingStrategy()
		{
			auto meta = voiceMetaData(voice());
			if (!defaultVoiceStrategy_) {
				defaultVoiceStrategy_ = fm::getWerckmeister().getDefaultVoicingStrategy();
			}
			if (!meta->voicingStrategy) {
				return defaultVoiceStrategy_;
			}
			return meta->voicingStrategy;
		}
		void AContext::setChord(const ChordEvent &chord)
		{
			currentChord_ = chord;
			currentChordDef_ = styleDefServer()->getChord(currentChord_.chordDefName());
			if (currentChordDef_ == nullptr) {
				throw std::runtime_error("chord not found: " + fm::to_string(currentChord_.chordName));
			}
		}

		void AContext::setTarget(const Track &track, const Voice &voice)
		{
			TrackId trackId;
			VoiceId voiceId;
			auto it = ptrIdMap_.find(&track);
			if (it == ptrIdMap_.end()) {
				trackId = createTrack();
				ptrIdMap_[&track] = trackId;
			}
			else {
				trackId = it->second;
			}
			it = ptrIdMap_.find(&voice);
			if (it == ptrIdMap_.end()) {
				voiceId = createVoice();
				ptrIdMap_[&voice] = voiceId;
			}
			else {
				voiceId = it->second;
			}
			setTarget(trackId, voiceId);
		}
		void AContext::renderStyle(fm::Ticks duration)
		{
			auto styleTracks = currentStyle();

			for (const auto &track : *styleTracks)
			{
				for (const auto &voice : track.voices)
				{
					if (voice.events.empty()) {
						continue;
					}
					setTarget(track, voice);
					auto meta = voiceMetaData(this->voice());
					fm::Ticks writtenDuration = 0;
					while ((duration - writtenDuration) > TickTolerance) { // loop until enough events are written
						auto it = voice.events.begin();
						if (meta->idxLastWrittenEvent >= 0) { // continue rendering
							it += meta->idxLastWrittenEvent;
							meta->idxLastWrittenEvent = -1;
							if (it->type == Event::EOB) {
								// happens: | r1 |  ->  | A B |
								// after a half rest the next event would be a new bar
								// its length check would produce a message
								++it;
								meta->barPosition = 0;
							}
						}
						else if (meta->eventOffset > 0) { // skip events (for upbeat)
							it += meta->eventOffset;
						}
						for (; it != voice.events.end(); ++it)
						{
							auto ev = *it;
							auto currentPos = meta->position;
							auto originalDuration = ev.duration;
							if (ev.isTimeConsuming() && meta->remainingTime > 0) {
								if (ev.duration == 0) {
									ev.duration = meta->duration;
								}
								ev.duration = ev.duration + meta->remainingTime;
								meta->remainingTime = 0;
							}
							ev.duration = std::min(ev.duration, duration - writtenDuration);
							addEvent(ev);
							writtenDuration += meta->position - currentPos;
							if ((duration - writtenDuration) <= TickTolerance) {
								bool hasRemainings = ev.duration != originalDuration;
								if (hasRemainings) {
									meta->remainingTime = originalDuration - ev.duration;
								}
								meta->idxLastWrittenEvent = it - voice.events.begin() + 1;
								break;
							}
						}
					}
				}
			}
		}
	}
}