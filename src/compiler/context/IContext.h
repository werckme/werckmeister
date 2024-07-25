#pragma once

#include <memory>
#include <unordered_map>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <map>
#include <unordered_map>
#include <com/common.hpp>
#include <list>
#include <forward.hpp>
#include <functional>
#include <compiler/IDefinitionsServer.h>
#include <compiler/timeInfo.h>
#include <compiler/metaData.h>
#include <compiler/Instrument.h>
#include <memory>
#include <set>

namespace compiler
{
	class ICompilerVisitor;
	class IContext
	{
	public:
		enum
		{
			INVALID_TRACK_ID = -1,
			INVALID_VOICE_ID = -1,
			MAX_VOLUME = 100,
			MAX_PAN = 100
		};
		typedef int Id;
		typedef Id TrackId;
		typedef Id VoiceId;
		typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
		typedef std::shared_ptr<TrackMetaData> TrackMetaDataPtr;
		typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
		typedef std::unordered_map<TrackId, TrackMetaDataPtr> TrackMetaDataMap;
		typedef std::function<void(com::String)> WarningHandler;
		typedef std::vector<documentModel::SheetTemplate> SheetTemplates;
		virtual void warningHandler(const WarningHandler &) = 0;
		virtual WarningHandler &warningHandler() = 0;
		virtual void setTrack(TrackId trackId) = 0;
		virtual void setVoice(VoiceId voice) = 0;
		virtual TrackId track() const = 0;
		/**
		 * @return the current voiceId
		 */
		virtual VoiceId voice() const = 0;
		virtual IContextPtr createNewContext() const = 0;
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
		virtual const SheetTemplates &currentSheetTemplates() = 0;
		virtual const VoicingStrategies& voicingStrategies() = 0;
		virtual AModificationPtr spielanweisung() = 0;
		virtual AInstrumentDefPtr getInstrumentDef(const com::String &uname) = 0;
		virtual AInstrumentDefPtr currentInstrumentDef() = 0;
		virtual const documentModel::Event& currentChordEvent() const = 0;
		virtual void currentChordEvent(const documentModel::Event&) = 0;
		virtual com::Ticks currentPosition() const = 0;
		virtual com::Ticks maxPosition() const = 0;
		virtual TimeInfo getTimeInfo() const = 0;
		virtual void setInstrument(const com::String &uname) = 0;
		virtual void setExpression(com::Expression value) = 0;
		virtual void setExpressionPlayedOnce(com::Expression expr) = 0;
		virtual void setTempo(double bpm) = 0;
		virtual void setSignature(int upper, int lower) = 0;
		virtual void setVolume(double volume, com::Ticks relativePosition = 0) = 0;
		virtual void setContinuousController(int controllerNumber, int value, com::Ticks relativePosition = 0, int prio = -1) = 0;
		virtual void setSysex(const com::Byte* data, size_t length, com::Ticks relativePosition = 0, int prio = -1) = 0;
		virtual void setPan(double val) = 0;
		virtual void addCue(const com::String &text, com::Ticks absolutePosition) = 0;
		/////// actual context stuff
		virtual void renderPitch(const documentModel::PitchDef &pitch, com::Ticks duration, double velocity, bool tying) = 0;
		virtual void renderPitch(const documentModel::PitchDef &pitch, com::Ticks absolutePosition, double velocity, com::Ticks duration) = 0;
		/*
			* value = 0..1, 0.5 is the middle position => no bending
			*/
		virtual void renderPitchbend(double value, com::Ticks absolutePosition) = 0;
		virtual void startEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition, double velocity) = 0;
		virtual void stopEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition) = 0;
		/**
		 * if duration == 0 the last event duration will be used
		 */
		virtual void seek(com::Ticks duration) = 0;
		virtual void newBar() = 0;
		virtual void rest(com::Ticks duration) = 0;
		virtual com::Ticks barPos() const = 0;
		/**
		 * the documents master tempo
		 */
		virtual double masterTempo() const = 0;
		virtual void masterTempo(double val) = 0;
		/**
		 * @return the current velocity value between 0..1
		 */
		virtual double velocity() = 0;
		virtual compiler::IDefinitionsServerPtr definitionsServer() = 0;
		/**
		 * clears all contents, resets settings  
		 */
		virtual void clear() = 0;
		/**
		 * sends note off to all pitches where its tie process wasn't completed yet
		 */
		virtual void stopAllPendingTies() = 0;
		/**
		 * retunrs the current compiler visitor
		 */
		virtual std::shared_ptr<ICompilerVisitor> compilerVisitor() const = 0;
		virtual void addTag(const com::String &tag) = 0;
		virtual void removeTag(const com::String &tag) = 0;
		virtual const VoiceMetaData::Tags & getTags() const = 0;
	};
	typedef std::shared_ptr<IContext> IContextPtr;

}
