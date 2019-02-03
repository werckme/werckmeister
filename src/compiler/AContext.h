#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include "sheet/ChordDef.h"
#include "sheet/StyleDefServer.h"
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"

namespace sheet {
    namespace compiler {
		struct AInstrumentDef {
			fm::String uname;
		};
        class AContext {
        public:
			AContext();
			static const double PitchbendMiddle;
			enum { INVALID_TRACK_ID = -1, INVALID_VOICE_ID = -1, MAX_VOLUME = 100, MAX_PAN = 100 };
			/**
			 * for rounding errors e.g. for triplets
			 */
			static const fm::Ticks TickTolerance;
			typedef int Id;
			typedef Id TrackId;
			typedef Id VoiceId;
			typedef IStyleDefServer* IStyleDefServerPtr;
			typedef std::list<std::string> Warnings;
			typedef std::unordered_map<fm::String, fm::Expression> ExpressionMap;
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::shared_ptr<TrackMetaData> TrackMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			typedef std::unordered_map<TrackId, TrackMetaDataPtr> TrackMetaDataMap;
			virtual void setTrack(TrackId trackId);
			virtual void setVoice(VoiceId voice);
			TrackId track() const;
			VoiceId voice() const;
			TrackId chordTrackId() const { return chordTrack_; }
			VoiceId chordVoiceId() const { return chordVoice_; }
			inline void setTarget(TrackId trackId, VoiceId voiceId)
			{
				setTrack(trackId);
				setVoice(voiceId);
			}
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) = 0;
			virtual ~AContext() {};
			/**
			 * creates a track and returns an id.
			 */
			virtual TrackId createTrack();
			/**
			 * creates a vocie and returns an id.
			 */			
			virtual VoiceId createVoice();
			virtual void setChordTrackTarget();
			/**
			 * @return the metdata for the current voice  
			 */
			VoiceMetaDataPtr voiceMetaData() const;
			VoiceMetaDataPtr voiceMetaData(VoiceId voiceid) const;
			template<typename TVoiceMeta>
			std::shared_ptr<TVoiceMeta> voiceMetaData(VoiceId voiceid) const 
			{
				return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData(voiceid));
			}
			template<typename TVoiceMeta>
			std::shared_ptr<TVoiceMeta> voiceMetaData() const 
			{
				return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData());
			}
			/**
			 * @return the metdata for the current track  
			 */
			TrackMetaDataPtr trackMetaData() const;
			TrackMetaDataPtr trackMetaData(TrackId trackid) const;
			template<typename TTrackMeta>
			std::shared_ptr<TTrackMeta> trackMetaData(TrackId trackid) const 
			{
				return std::dynamic_pointer_cast<TTrackMeta>(trackMetaData(trackid));
			}
			template<typename TTrackMeta>
			std::shared_ptr<TTrackMeta> trackMetaData() const 
			{
				return std::dynamic_pointer_cast<TTrackMeta>(trackMetaData());
			}					
			virtual void throwContextException(const std::string &msg);
			virtual void warn(const std::string &msg);
			IStyleDefServerPtr styleDefServer() const;
			void styleDefServer(IStyleDefServerPtr server);
			virtual IStyleDefServer::ConstChordValueType currentChordDef();
			virtual IStyleDefServer::ConstStyleValueType currentStyle();
			virtual void currentStyle(IStyleDefServer::ConstStyleValueType style);
			virtual VoicingStrategyPtr currentVoicingStrategy();
			virtual const ChordEvent * currentChord() const { return &currentChord_; }
			virtual fm::Expression getExpression(const fm::String &str) const;
			virtual ASpielanweisungPtr spielanweisung();
			virtual AInstrumentDef * getInstrumentDef(const fm::String &uname) = 0;
			virtual fm::Ticks currentPosition() const;
			virtual void processTrackMetaData(const sheet::Track &track);
			virtual void processTrackMetaCommand(const fm::String &command, const sheet::TrackInfo::Args &args, TrackMetaDataPtr dst = nullptr);
			/////// meta commands
			virtual void setMeta(const Event &metaEvent);
			virtual void metaSetInstrument(const fm::String &uname) {}
			virtual void metaSetStyle(const fm::String &file, const fm::String &section);
			virtual void metaSetExpression(const fm::String &value);
			virtual void metaSetSingleExpression(const fm::String &value);
			virtual void metaSetTempo(double bpm) {}
			virtual void metaSetUpbeat(const Event &event);
			virtual void metaSetVoicingStrategy(const fm::String &name, const Event::Args &args);
			virtual void metaSetSpielanweisung(const fm::String &name, const Event::Args &args);
			virtual void metaSetSpielanweisungOnce(const fm::String &name, const Event::Args &args);
			virtual void metaSetModification(const fm::String &name, const Event::Args &args);
			virtual void metaSetModificationOnce(const fm::String &name, const Event::Args &args);
			virtual void metaSetSignature(int upper, int lower);
			virtual void metaAddDevice(const fm::String name, const Event::Args &args);
			virtual void metaAddVorschlag(const Event &ev);
			virtual void metaSetVolume(int volume);
			virtual void metaSetPan(int val);
			/////// actual context stuff
			virtual void addEvent(const Event::Pitches &pitches, fm::Ticks duration, bool tying = false);
			virtual void addEvent(const PitchDef &pitch, fm::Ticks duration, bool tying = false);
			/*
			 * value = 0..1
			 */
			virtual void addPitchbendEvent(double value, fm::Ticks absolutePosition) = 0;			
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition);
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition);
			/**
			 * if duration == 0 the last event duration will be used
			 */ 
			virtual void seek(fm::Ticks duration);
			virtual void newBar();
			virtual void rest(fm::Ticks duration);
			virtual void setChord(const ChordEvent &ev);
			virtual void addEvent(const Event &ev);
			virtual void stopTying();
			virtual fm::Ticks barPos() const;
			Warnings warnings;
		protected:
			PitchDef resolvePitch(const PitchDef &pitch) const;
			virtual TrackId createTrackImpl() = 0;
			virtual VoiceId createVoiceImpl() = 0;
			virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
			virtual TrackMetaDataPtr createTrackMetaData() = 0;
		private:
			ChordEvent currentChord_;
			VoicingStrategyPtr defaultVoiceStrategy_;
			IStyleDefServer::ConstChordValueType currentChordDef_ = nullptr;
			IStyleDefServer::ConstStyleValueType currentStyleDef_ = nullptr;
			TrackId trackId_ = INVALID_TRACK_ID, chordTrack_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID, chordVoice_ = INVALID_VOICE_ID;
			VoiceMetaDataMap voiceMetaDataMap_;
			TrackMetaDataMap trackMetaDataMap_;
			IStyleDefServerPtr styleDefServer_ = nullptr;
			ExpressionMap expressionMap_;
			ASpielanweisungPtr defaultSpielanweisung_;
        };	
    }
}

#endif