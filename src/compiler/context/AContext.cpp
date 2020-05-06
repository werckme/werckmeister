#include "AContext.h"
#include <compiler/error.hpp>
#include <fm/werckmeister.hpp>
#include <algorithm>
#include <fm/common.hpp>
#include <compiler/spielanweisung/ASpielanweisung.h>
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/spielanweisung/Vorschlag.h>
#include <compiler/modification/AModification.h>
#include <fm/literals.hpp>
#include <fm/config/configServer.h>
#include <sheet/Track.h>
#include <sheet/tools.h>
#include <sstream>

namespace sheet {

	namespace compiler {
		using namespace fm;
		const double AContext::PitchbendMiddle = 0.5;
		const Ticks AContext::TickTolerance = 0.5;
		
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

		fm::Expression AContext::getExpression(const fm::String &str) const
		{
			auto it = expressionMap_.find(str);
			if (it == expressionMap_.end()) {
				return fm::expression::Default;
			}
			return it->second;
		}

		AContext::ISheetTemplateDefServerPtr AContext::sheetTemplateDefServer() const
		{
			if (!sheetTemplateDefServer_) {
				FM_THROW(Exception, "no sheetTemplatedef server set");
			}
			return sheetTemplateDefServer_;
		}
		void AContext::sheetTemplateDefServer(ISheetTemplateDefServerPtr server)
		{
			sheetTemplateDefServer_ = server;
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

		PitchDef AContext::resolvePitch(const PitchDef &pitch) const
		{
			if (pitch.alias.empty()) {
				return pitch;
			}
			const PitchDef *result = sheetTemplateDefServer()->getAlias(pitch.alias);
			if (result == nullptr) {
				FM_THROW(Exception, "could not resolve alias: " + pitch.alias);
			}
			return *result;
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
			PitchDef pitch = resolvePitch(rawPitch);
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

		void AContext::warn(const std::string &msg, const Event *event)
		{
			Warning warning;
			warning.message = msg;
			if (!!event) {
				warning.sourceObject = *event;
			}
			warnings.emplace_back(warning);
		}

		void AContext::newBar(const Event &newBarEvent)
		{
			auto meta = voiceMetaData();
			if (!fm::compareTolerant(meta->barPosition, meta->barLength, fm::Ticks(TickTolerance))) {
				auto errorInQuaters = -(meta->barLength - meta->barPosition) / fm::PPQ;
				seek(-(meta->barPosition - meta->barLength));
				std::stringstream ss;
				std::string toLong = errorInQuaters > 0 ? "too long" : "too short";
				ss << "bar is " << std::abs(errorInQuaters) << " quarter(s) " << toLong;
				warn(ss.str(), &newBarEvent);
			}
			meta->barPosition = 0;
			++(meta->barCount);
		}

		void AContext::rest(fm::Ticks duration)
		{
			auto meta = voiceMetaData();
			seek(duration);
		}
		void AContext::processMeta(const fm::String &command, const std::vector<sheet::Argument> &args)
		{
			// #74 TODO
			// try {
			// 	if (command == SHEET_META__TRACK_META_KEY_TYPE /*handled elsewhere*/
			// 	|| command == SHEET_META__TRACK_META_KEY_NAME
			// 	|| command == SHEET_META__SET_VORSCHLAG
			// 	|| command == SHEET_META__SHEET_TEMPLATE_POSITION) 
			// 	{
			// 		return;
			// 	}
							
			// 	if (command == SHEET_META__SET_SHEET_TEMPLATE) {
			// 		metaSetSheetTemplate(args);
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_EXPRESSION) {
			// 		metaSetExpression(getArgument<fm::String>(args, 0));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_SINGLE_EXPRESSION) {
			// 		metaSetSingleExpression(getArgument<fm::String>(args, 0));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_TEMPO) {
			// 		if (args.front() == SHEET_META__SET_TEMPO_VALUE_HALF) {
			// 			metaSetTempo(masterTempo() * 0.5);
			// 			return;
			// 		}
			// 		if (args.front() == SHEET_META__SET_TEMPO_VALUE_DOUBLE) {
			// 			metaSetTempo(masterTempo() * 2);
			// 			return;
			// 		}
			// 		if (args.front() == SHEET_META__SET_TEMPO_VALUE_NORMAL) {
			// 			metaSetTempo(masterTempo());
			// 			return;
			// 		}					
			// 		metaSetTempo(getArgument<fm::BPM>(args, 0));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_VOICING_STRATEGY) {
			// 		metaSetVoicingStrategy(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_SPIELANWEISUNG) {
			// 		metaSetSpielanweisung(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}	
			// 	if (command == SHEET_META__SET_SPIELANWEISUNG_ONCE) {
			// 		metaSetSpielanweisungOnce(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}	
			// 	if (command == SHEET_META__SET_MOD) {
			// 		metaSetModification(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}	
			// 	if (command == SHEET_META__SET_MOD_ONCE) {
			// 		metaSetModificationOnce(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_SIGNATURE) {
			// 		metaSetSignature(getArgument<int>(args, 0), getArgument<int>(args, 1));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_DEVICE) {
			// 		metaAddDevice(getArgument<fm::String>(args, 0), args);
			// 		return;
			// 	}	
			// 	if (command == SHEET_META__SET_VOLUME) {
			// 		metaSetVolume(getArgument<int>(args, 0));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__SET_PAN) {
			// 		metaSetPan(getArgument<int>(args, 0));
			// 		return;
			// 	}
			// 	if (command == SHEET_META__INSTRUMENT) {
			// 		metaSetInstrument(getArgument<fm::String>(args, 0));
			// 		return;
			// 	}
			// } catch(const std::exception &ex) {
			// 	FM_THROW(Exception, "failed to process " + command
			// 						+": " + ex.what());
			// }	
			// catch(...) {
			// 	FM_THROW(Exception, "failed to process " + command);
			// }
			// FM_THROW(Exception, "invalid command: " + command);								
		}
		void AContext::setMeta(const Event &metaEvent)
		{
			if (metaEvent.stringValue.empty()) {
				throwContextException("invalid meta command ");
			}
			if (metaEventHandler && metaEventHandler(metaEvent)) {
				return;
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
			// #74 TODO
			// auto &cs = getConfigServer();
			// if (args.size() < 2) {
			// 	FM_THROW(Exception, "not enough arguments for device config");
			// }
			// std::vector<fm::String> deviceArgs(args.begin() + 1, args.end());
			// auto device = cs.createDeviceConfig(name, deviceArgs);
			// cs.addDevice(name, device);
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

		void AContext::metaSetSheetTemplate(const Event::Args &)
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

		void AContext::metaSetSignature(int upper, int lower)
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
				auto defaultTemplate = sheetTemplateDefServer()->getSheetTemplate(FM_STRING("?"));
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
	}
}