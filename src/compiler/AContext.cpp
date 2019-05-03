#include "AContext.h"
#include "error.hpp"
#include <fm/werckmeister.hpp>
#include <algorithm>
#include <fm/common.hpp>
#include "spielanweisung/ASpielanweisung.h"
#include "spielanweisung/spielanweisungen.h"
#include "spielanweisung/Vorschlag.h"
#include "modification/AModification.h"
#include <fm/literals.hpp>
#include <fm/config/configServer.h>
#include <sheet/Track.h>
#include "sheet/tools.h"

namespace sheet {

	namespace compiler {
		using namespace fm;
		const double AContext::PitchbendMiddle = 0.5;
		const Ticks AContext::TickTolerance = 0.5;
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
			bool renderEvent<Event::Chord>(AContext * ctx, const Event *chordEv)
			{
				ctx->setChord(*chordEv);
				ctx->seek(chordEv->duration);
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

		AContext::TrackId AContext::createMasterTrack()
		{
			this->masterTrackId_ = createTrack();
			return this->masterTrackId_;
		}

		ASpielanweisungPtr AContext::spielanweisung()
		{
			auto meta = voiceMetaData();
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
				FM_THROW(Exception, "no styledef server set");
			}
			return styleDefServer_;
		}
		void AContext::styleDefServer(IStyleDefServerPtr server)
		{
			styleDefServer_ = server;
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

		PitchDef AContext::resolvePitch(const PitchDef &pitch) const
		{
			if (pitch.alias.empty()) {
				return pitch;
			}
			const PitchDef *result = styleDefServer()->getAlias(pitch.alias);
			if (result == nullptr) {
				FM_THROW(Exception, "could not resolve alias: " + pitch.alias);
			}
			return *result;
		}

		fm::Ticks AContext::getImlplicitDuration(const Event &ev) const
		{
			if (ev.duration > 0) {
				return ev.duration;
			}
			auto meta = voiceMetaData();
			return meta->lastEventDuration;
		}

		void AContext::addEvent(const PitchDef &rawPitch, fm::Ticks duration, bool tying)
		{
			using namespace fm;
			PitchDef pitch = resolvePitch(rawPitch);
			auto meta = voiceMetaData();
			if (duration > 0) {
				meta->lastEventDuration = duration;
			}
			if (tying) {
				auto alreadyTying = meta->waitForTieBuffer.find(pitch) != meta->waitForTieBuffer.end();
				if (!alreadyTying) {
					meta->waitForTieBuffer.insert({ pitch, meta->lastEventDuration });
					startEvent(pitch, meta->position);
				}
				return;
			}
			if (meta->pendingTie()) {
				auto it = meta->waitForTieBuffer.find(pitch);
				if (it != meta->waitForTieBuffer.end()) {
					stopEvent(pitch, meta->position + meta->lastEventDuration);
					meta->waitForTieBuffer.erase(it);
					return;
				}
			}
			addEvent(pitch, meta->position, meta->lastEventDuration);
		}

		void AContext::stopTying()
		{
			auto meta = voiceMetaData();
			for(auto tied : meta->startedEvents){
				stopEvent(tied, meta->position + meta->lastEventDuration);
			}
		}

		void AContext::startEvent(const PitchDef &pitch, fm::Ticks absolutePosition)
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

		void AContext::addEvent(const Event &ev)
		{
			auto meta = voiceMetaData();
			++(meta->eventCount);
			try {
				_addEvent(this, &ev);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_source_info(ev);
				throw;
			}
		}

		void AContext::addEvent(const Event::Pitches &pitches, fm::Ticks duration, bool tying)
		{
			auto meta = voiceMetaData();
			auto tmpExpression = meta->expression;

			if (meta->singleExpression != fm::expression::Default) {
				tmpExpression = meta->expression;
				meta->expression = meta->singleExpression;
				meta->singleExpression = fm::expression::Default;
			}
			if (duration == 0) {
				duration = meta->lastEventDuration;
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
			auto meta = voiceMetaData();
			meta->position += duration;
			meta->barPosition += duration;
		}

		void AContext::warn(const std::string &msg)
		{
			auto tmeta = trackMetaData();
			auto vmeta = voiceMetaData();
			std::string voiceName;
			fm::Ticks pos = 0;
			if (tmeta) {
				voiceName = tmeta->instrument.empty() ? std::to_string(voice()) : tmeta->instrument;
			}
			if (vmeta) {
				pos = vmeta->position / vmeta->barLength;
			}
			std::string warning(msg + " at '" + voiceName + "', bar: " + std::to_string(pos));
			warnings.push_back(warning);
		}

		void AContext::newBar()
		{
			auto meta = voiceMetaData();
			if (meta->isUpbeat) {
				meta->isUpbeat = false;
				meta->eventOffset = meta->eventCount;
			}
			else if (!fm::compareTolerant(meta->barPosition, meta->barLength, fm::Ticks(TickTolerance))) {
				auto errorInQuaters = -(meta->barLength - meta->barPosition) / fm::PPQ;
				warn("bar check error (" + std::to_string( errorInQuaters )+ ")");
				seek(-(meta->barPosition - meta->barLength));
			}
			meta->barPosition = 0;
			++(meta->barCount);
		}

		void AContext::rest(fm::Ticks duration)
		{
			auto meta = voiceMetaData();
			if (duration != 0) {
				meta->lastEventDuration = duration;
			}
			seek(meta->lastEventDuration);
		}
		void AContext::processMeta(const fm::String &command, const std::vector<fm::String> &args)
		{
			try {
				if (command == SHEET_META__TRACK_META_KEY_TYPE /*handled elsewhere*/
				|| command == SHEET_META__TRACK_META_KEY_NAME
				|| command == SHEET_META__TRACK_META_KEY_PART
				|| command == SHEET_META__SET_VORSCHLAG
				|| command == SHEET_META__SET_UPBEAT) 
				{
					return;
				}
							
				if (command == SHEET_META__SET_STYLE) {
					metaSetStyle(getArgument<fm::String>(args, 0), getArgument<fm::String>(args, 1));
					return;
				}
				if (command == SHEET_META__SET_EXPRESSION) {
					metaSetExpression(getArgument<fm::String>(args, 0));
					return;
				}
				if (command == SHEET_META__SET_SINGLE_EXPRESSION) {
					metaSetSingleExpression(getArgument<fm::String>(args, 0));
					return;
				}
				if (command == SHEET_META__SET_TEMPO) { 
					metaSetTempo(getArgument<fm::BPM>(args, 0));
					return;
				}
				if (command == SHEET_META__SET_VOICING_STRATEGY) {
					metaSetVoicingStrategy(getArgument<fm::String>(args, 0), args);
					return;
				}
				if (command == SHEET_META__SET_SPIELANWEISUNG) {
					metaSetSpielanweisung(getArgument<fm::String>(args, 0), args);
					return;
				}	
				if (command == SHEET_META__SET_SPIELANWEISUNG_ONCE) {
					metaSetSpielanweisungOnce(getArgument<fm::String>(args, 0), args);
					return;
				}	
				if (command == SHEET_META__SET_MOD) {
					metaSetModification(getArgument<fm::String>(args, 0), args);
					return;
				}	
				if (command == SHEET_META__SET_MOD_ONCE) {
					metaSetModificationOnce(getArgument<fm::String>(args, 0), args);
					return;
				}
				if (command == SHEET_META__SET_SIGNATURE) {
					metaSetSignature(getArgument<int>(args, 0), getArgument<int>(args, 1));
					return;
				}
				if (command == SHEET_META__SET_DEVICE) {
					metaAddDevice(getArgument<fm::String>(args, 0), args);
					return;
				}	
				if (command == SHEET_META__SET_VOLUME) {
					metaSetVolume(getArgument<int>(args, 0));
					return;
				}
				if (command == SHEET_META__SET_PAN) {
					metaSetPan(getArgument<int>(args, 0));
					return;
				}
				if (command == SHEET_META__INSTRUMENT) {
					metaSetInstrument(getArgument<fm::String>(args, 0));
					return;
				}
			} catch(const std::exception &ex) {
				FM_THROW(Exception, "failed to process " + command
									+": " + ex.what());
			}	
			catch(...) {
				FM_THROW(Exception, "failed to process " + command);
			}
			FM_THROW(Exception, "command not found: " + command);								
		}
		void AContext::setMeta(const Event &metaEvent)
		{
			if (metaEvent.stringValue.empty()) {
				throwContextException("invalid meta command ");
			}
			if (metaEventHandler && metaEventHandler(metaEvent)) {
				return;
			}
			if (metaEvent.stringValue == SHEET_META__SET_UPBEAT) { 
				metaSetUpbeat(metaEvent);
			}
			if (metaEvent.stringValue == SHEET_META__SET_VORSCHLAG) {
				metaAddVorschlag(metaEvent);
			}
			processMeta(metaEvent.stringValue, metaEvent.metaArgs);				
		}

		void AContext::metaSetVolume(int volume)
		{
			auto meta = voiceMetaData();
			meta->volume = std::max(std::min(volume, 100), 0);
		}

		void AContext::metaSetPan(int val)
		{
			auto meta = voiceMetaData();
			meta->pan = std::max(std::min(val, 100), 0);
		}		

		void AContext::metaAddVorschlag(const Event &ev)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData();
			meta->spielanweisungOnce = wm.getSpielanweisung(SHEET_SPIELANWEISUNG_VORSCHLAG);
			auto vorschlag = std::dynamic_pointer_cast<Vorschlag>(meta->spielanweisungOnce);
			vorschlag->vorschlagNote = ev;
		}

		void AContext::metaAddDevice(const fm::String name, const Event::Args &args)
		{
			auto &cs = getConfigServer();
			if (args.size() < 2) {
				FM_THROW(Exception, "not enough arguments for device config");
			}
			std::vector<fm::String> deviceArgs(args.begin() + 1, args.end());
			auto device = cs.createDeviceConfig(name, deviceArgs);
			cs.addDevice(name, device);
		}

		void AContext::metaSetVoicingStrategy(const fm::String &name, const Event::Args &args)
		{
			auto meta = voiceMetaData();
			if (meta->voicingStrategy && meta->voicingStrategy->name() == name) {
				return;
			}
			auto &wm = fm::getWerckmeister();
			meta->voicingStrategy = wm.getVoicingStrategy(name);
			meta->voicingStrategy->setArguments(args);
		}

		void AContext::metaSetSpielanweisung(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData();
			meta->spielanweisung = wm.getSpielanweisung(name);
			meta->spielanweisung->setArguments(args);
		}

		void AContext::metaSetSpielanweisungOnce(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData();
			meta->spielanweisungOnce = wm.getSpielanweisung(name);
			meta->spielanweisungOnce->setArguments(args);
		}

		void AContext::metaSetModification(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData();
			auto mod = wm.getModification(name);
			meta->modifications.push_back(mod);
			mod->setArguments(args);
		}

		void AContext::metaSetModificationOnce(const fm::String &name, const Event::Args &args)
		{
			auto &wm = fm::getWerckmeister();
			auto meta = voiceMetaData();
			auto mod = wm.getModification(name);
			meta->modificationsOnce.push_back(mod);
			mod->setArguments(args);
		}

		void AContext::metaSetStyle(const fm::String &file, const fm::String &part)
		{
		}

		void AContext::metaSetExpression(const fm::String &value)
		{
			auto meta = voiceMetaData();
			auto expr = getExpression(value);
			if (expr == fm::expression::Default) {
				return;
			}
			meta->expression = expr;
		}

		void AContext::metaSetSingleExpression(const fm::String &value)
		{
			auto meta = voiceMetaData();
			auto expr = getExpression(value);
			if (expr == fm::expression::Default) {
				return;
			}
			meta->singleExpression = expr;
		}

		void AContext::metaSetUpbeat(const Event &event) 
		{
			auto meta = voiceMetaData();
			if (meta->position > 0) {
				throwContextException("upbeat only allowed on begin of track");
			}
			meta->isUpbeat = true;
		}

		void AContext::metaSetSignature(int upper, int lower)
		{
			using namespace fm;
			auto meta = voiceMetaData();
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

		/////////////////////////////////////////////////////////////////////////////
		// Stylerendering
		IStyleDefServer::ConstChordValueType AContext::currentChordDef()
		{
			if (!currentChordDef_) {
				currentChordDef_ = styleDefServer()->getChord(FM_STRING("?"));
			}
			return currentChordDef_;
		}
		IStyleDefServer::Style AContext::currentStyle()
		{
			if (currentStyle_.empty()) {
				currentStyle_ = styleDefServer()->getStyle(FM_STRING("?"));
			}
			return currentStyle_;
		}
		void AContext::currentStyle(const IStyleDefServer::Style &style)
		{
			currentStyle_ = style;
		}
		VoicingStrategyPtr AContext::currentVoicingStrategy()
		{
			auto meta = voiceMetaData();
			if (!defaultVoiceStrategy_) {
				defaultVoiceStrategy_ = fm::getWerckmeister().getDefaultVoicingStrategy();
			}
			if (!meta->voicingStrategy) {
				return defaultVoiceStrategy_;
			}
			return meta->voicingStrategy;
		}
		void AContext::setChord(const Event &chord)
		{
			currentChord_ = chord;
			currentChordDef_ = styleDefServer()->getChord(currentChord_.chordDefName());
			if (currentChordDef_ == nullptr) {
				FM_THROW(Exception, "chord not found: " + currentChord_.stringValue);
			}
		}
	}
}