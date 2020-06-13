#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include <fm/IDefinitionsServer.h>
#include <fm/common.hpp>
#include <compiler/metaCommands.h>
#include <list>
#include <forward.hpp>
#include <compiler/metaData.h>
#include <compiler/error.hpp>
#include <compiler/instrument.h>
#include <compiler/timeInfo.h>
#include "IContext.h"

namespace sheet {
    namespace compiler {
        class AContext : public IContext {
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
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::shared_ptr<TrackMetaData> TrackMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			typedef std::unordered_map<TrackId, TrackMetaDataPtr> TrackMetaDataMap;
			typedef std::function<void(fm::String)> WarningHandler;
			virtual void setTrack(TrackId trackId);
			virtual void setVoice(VoiceId voice);
			TrackId track() const;
			/**
			 * @return the current voiceId
			 */
			VoiceId voice() const;
			TrackId chordTrackId() const { return chordTrack_; }
			TrackId masterTrackId() 
			{ 
				if (masterTrackId_ == INVALID_TRACK_ID) {
					this->createMasterTrack();
				}
				return masterTrackId_; 
			}
			VoiceId chordVoiceId() const { return chordVoice_; }
			inline void setTarget(TrackId trackId, VoiceId voiceId)
			{
				setTrack(trackId);
				setVoice(voiceId);
			}
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
			WarningHandler warningHandler;
			virtual const SheetTemplates & currentSheetTemplates();
			virtual void currentSheetTemplate(const SheetTemplates &sheetTemplate);
			virtual VoicingStrategyPtr currentVoicingStrategy();
			virtual AModificationPtr spielanweisung();
			virtual AInstrumentDef * getInstrumentDef(const fm::String &uname) = 0;
			virtual AInstrumentDef * currentInstrumentDef() = 0;
			virtual fm::Ticks currentPosition() const;
			virtual fm::Ticks maxPosition() const;
			TimeInfo getTimeInfo() const;
			virtual void setInstrument(const fm::String &uname) {}
			virtual void setExpression(fm::Expression value);
			virtual void setExpressionPlayedOnce(fm::Expression expr);
			virtual void setTempo(double bpm) {}
			virtual void setSignature(int upper, int lower);
			virtual void setVolume(double volume);
			virtual void setPan(double val);
			/////// actual context stuff
			virtual void renderPitch(const PitchDef &pitch, fm::Ticks duration, double velocity, bool tying);
			virtual void renderPitch(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity, fm::Ticks duration) = 0;
			/*
			 * value = 0..1, 0.5 is the middle position => no bending
			 */
			virtual void renderPitchbend(double value, fm::Ticks absolutePosition) = 0;			
			virtual void startEvent(const PitchDef &pitch, fm::Ticks absolutePosition, double velocity);
			virtual void stopEvent(const PitchDef &pitch, fm::Ticks absolutePosition);
			/**
			 * if duration == 0 the last event duration will be used
			 */ 
			virtual void seek(fm::Ticks duration);
			virtual void newBar();
			virtual void rest(fm::Ticks duration);
			virtual fm::Ticks barPos() const;
			/**
			 * the documents master tempo
			 */
			virtual double masterTempo() const { return masterTempo_; }
			virtual void masterTempo(double val) { this->masterTempo_ = val; }			
			Warnings warnings;
            /**
             * @return the current velocity value between 0..1
             */
            virtual double velocity();				
		protected:
			virtual TrackId createTrackImpl() = 0;
			virtual VoiceId createVoiceImpl() = 0;
			virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
			virtual TrackMetaDataPtr createTrackMetaData() = 0;
			virtual TrackId createMasterTrack();
		private:
			double masterTempo_ = fm::DefaultTempo;			
			VoicingStrategyPtr defaultVoiceStrategy_;
			SheetTemplates currentSheetTemplates_;
			TrackId trackId_ = INVALID_TRACK_ID, 
				 chordTrack_ = INVALID_TRACK_ID,
				 masterTrackId_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID, chordVoice_ = INVALID_VOICE_ID;
			VoiceMetaDataMap voiceMetaDataMap_;
			TrackMetaDataMap trackMetaDataMap_;
        };
    }
}

#endif