#ifndef COMPILER_ICONTEXT_H
#define COMPILER_ICONTEXT_H

#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include <fm/common.hpp>
#include <list>
#include <forward.hpp>
#include <functional>
#include <fm/SheetTemplateDefServer.h>
#include <compiler/timeInfo.h>
#include <compiler/metaData.h>
#include <compiler/instrument.h>

namespace sheet {
    namespace compiler {
        class IContext {
        public:
			typedef int Id;
			typedef Id TrackId;
			typedef Id VoiceId;
			typedef ISheetTemplateDefServer* ISheetTemplateDefServerPtr;
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::shared_ptr<TrackMetaData> TrackMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			typedef std::unordered_map<TrackId, TrackMetaDataPtr> TrackMetaDataMap;
			typedef std::function<void(fm::String)> WarningHandler;
			typedef std::vector<ISheetTemplateDefServer::SheetTemplate> SheetTemplates;
			virtual void setTrack(TrackId trackId) = 0;
			virtual void setVoice(VoiceId voice) = 0;
			virtual TrackId track() const = 0;
			/**
			 * @return the current voiceId
			 */
			virtual VoiceId voice() const = 0;
			virtual TrackId chordTrackId() const = 0;
			virtual TrackId masterTrackId() = 0;
			virtual VoiceId chordVoiceId() const = 0;
			virtual void setTarget(TrackId trackId, VoiceId voiceId) = 0;
			/**
			 * creates a track and returns an id.
			 */
			virtual TrackId createTrack() = 0;
			/**
			 * creates a vocie and returns an id.
			 */			
			virtual VoiceId createVoice() = 0;
			virtual void setChordTrackTarget() = 0;
			/**
			 * @return the metdata for the current voice  
			 */
			virtual VoiceMetaDataPtr voiceMetaData() const = 0;
			virtual VoiceMetaDataPtr voiceMetaData(VoiceId voiceid) const = 0;
			/**
			 * @return the metdata for the current track  
			 */
			virtual TrackMetaDataPtr trackMetaData() const = 0;
			virtual TrackMetaDataPtr trackMetaData(TrackId trackid) const = 0;
			/**
			 * TODO: no context domain
			 */
			virtual ISheetTemplateDefServerPtr sheetTemplateDefServer() const = 0;
			/**
			 * TODO: no context domain
			 */
			virtual const SheetTemplates & currentSheetTemplates() = 0;
			/**
			 * TODO: no context domain
			 */			
			virtual void currentSheetTemplate(const SheetTemplates &sheetTemplate) = 0;
			virtual VoicingStrategyPtr currentVoicingStrategy() = 0;
			virtual AModificationPtr spielanweisung() = 0;
			virtual AInstrumentDef * getInstrumentDef(const fm::String &uname) = 0;
			virtual AInstrumentDef * currentInstrumentDef() = 0;
			virtual fm::Ticks currentPosition() const = 0;
			virtual fm::Ticks maxPosition() const = 0;
			virtual TimeInfo getTimeInfo() const = 0;
			virtual void setInstrument(const fm::String &uname) = 0;
			virtual void setExpression(fm::Expression value) = 0;
			virtual void setExpressionPlayedOnce(fm::Expression expr) = 0;
			virtual void setTempo(double bpm) = 0;
			virtual void setSignature(int upper, int lower) = 0;
			virtual void setVolume(double volume) = 0;
			virtual void setPan(double val) = 0;
			/////// actual context stuff
			virtual void renderPitch(const PitchDef &pitch, fm::Ticks duration, double velocity, bool tying) = 0;
			virtual void renderPitch(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity, fm::Ticks duration) = 0;
			/*
			 * value = 0..1, 0.5 is the middle position => no bending
			 */
			virtual void renderPitchbend(double value, fm::Ticks absolutePosition) = 0;			
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity) = 0;
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition) = 0;
			/**
			 * if duration == 0 the last event duration will be used
			 */ 
			virtual void seek(fm::Ticks duration) = 0;
			virtual void newBar() = 0;
			virtual void rest(fm::Ticks duration) = 0;
			virtual fm::Ticks barPos() const = 0;
			/**
			 * the documents master tempo
			 */
			virtual double masterTempo() const = 0;
			virtual void masterTempo(double val) = 0;
            /**
             * @return the current velocity value between 0..1
             */
            virtual double velocity() = 0;
			/**
			 * resolve pitches alias if exists
			 * TODO: no context domain
			 */
			virtual PitchDef resolvePitch(const PitchDef &pitch) const = 0;						
        };
    }
}

#endif