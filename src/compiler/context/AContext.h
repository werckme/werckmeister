#pragma once

#include <memory>
#include <unordered_map>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <map>
#include <unordered_map>
#include <compiler/IDefinitionsServer.h>
#include <com/common.hpp>
#include <compiler/metaCommands.h>
#include <list>
#include <forward.hpp>
#include <compiler/metaData.h>
#include <compiler/error.hpp>
#include <compiler/Instrument.h>
#include <compiler/timeInfo.h>
#include "IContext.h"
#include <compiler/ICompilerVisitor.h>

namespace compiler
{
	class AContext : public IContext
	{
	public:
		AContext(compiler::IDefinitionsServerPtr definitionsServer, ICompilerVisitorPtr compilerVisitor);
		static const double PitchbendMiddle;
		/**
		 * for rounding errors e.g. for triplets
		 */
		static const com::Ticks TickTolerance;
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
			if (masterTrackId_ == INVALID_TRACK_ID)
			{
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
		virtual ~AContext(){};
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
		template <typename TVoiceMeta>
		std::shared_ptr<TVoiceMeta> voiceMetaData(VoiceId voiceid) const
		{
			return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData(voiceid));
		}
		template <typename TVoiceMeta>
		std::shared_ptr<TVoiceMeta> voiceMetaData() const
		{
			return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData());
		}
		/**
		 * @return the metdata for the current track  
		 */
		TrackMetaDataPtr trackMetaData() const;
		TrackMetaDataPtr trackMetaData(TrackId trackid) const;
		template <typename TTrackMeta>
		std::shared_ptr<TTrackMeta> trackMetaData(TrackId trackid) const
		{
			return std::dynamic_pointer_cast<TTrackMeta>(trackMetaData(trackid));
		}
		template <typename TTrackMeta>
		std::shared_ptr<TTrackMeta> trackMetaData() const
		{
			return std::dynamic_pointer_cast<TTrackMeta>(trackMetaData());
		}
		virtual void throwContextException(const std::string &msg);
		virtual void warn(const std::string &msg);
		virtual const SheetTemplates &currentSheetTemplates();
		virtual void currentSheetTemplate(const SheetTemplates &sheetTemplate);
		virtual const VoicingStrategies& voicingStrategies() override;
		virtual const documentModel::Event& currentChordEvent() const override;
		virtual void currentChordEvent(const documentModel::Event&);
		virtual AModificationPtr spielanweisung();
		virtual AInstrumentDefPtr getInstrumentDef(const com::String &uname) = 0;
		virtual AInstrumentDefPtr currentInstrumentDef() = 0;
		virtual com::Ticks currentPosition() const;
		virtual com::Ticks maxPosition() const;
		TimeInfo getTimeInfo() const;
		virtual void setInstrument(const com::String &uname);
		virtual void setExpression(com::Expression value);
		virtual void setExpressionPlayedOnce(com::Expression expr);
		virtual void setTempo(double bpm) {}
		virtual void setSignature(int upper, int lower);
		virtual void setVolume(double volume, com::Ticks relativePosition = 0);
		virtual void setPan(double val);
		/////// actual context stuff
		virtual void renderPitch(const documentModel::PitchDef &pitch, com::Ticks duration, double velocity, bool tying);
		virtual void renderPitch(const documentModel::PitchDef &pitch, com::Ticks absolutePosition, double velocity, com::Ticks duration) = 0;
		/*
			* value = 0..1, 0.5 is the middle position => no bending
			*/
		virtual void renderPitchbend(double value, com::Ticks absolutePosition) = 0;
		virtual void startEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition, double velocity);
		virtual void stopEvent(const documentModel::PitchDef &pitch, com::Ticks absolutePosition);
		/**
		 * sends note off to all pitches where its tie process wasn't completed yet
		 */
		virtual void stopAllPendingTies() override;
		/**
		 * if duration == 0 the last event duration will be used
		 */
		virtual void seek(com::Ticks duration);
		virtual void newBar();
		virtual void rest(com::Ticks duration);
		virtual com::Ticks barPos() const;
		/**
		 * the documents master tempo
		 */
		virtual double masterTempo() const { return masterTempo_; }
		virtual void masterTempo(double val) { this->masterTempo_ = val; }
		/**
		 * @return the current velocity value between 0..1
		 */
		virtual double velocity();
		virtual compiler::IDefinitionsServerPtr definitionsServer() { return definitionsServer_; }
		virtual void warningHandler(const WarningHandler &handler) { _warningHandler = handler; }
		virtual WarningHandler &warningHandler() { return _warningHandler; }
		virtual void clear() override;
		virtual void addTag(const com::String &tag) override;
		virtual void removeTag(const com::String &tag) override;
		virtual const VoiceMetaData::Tags & getTags() const override;
	protected:
		virtual TrackId createTrackImpl() = 0;
		virtual VoiceId createVoiceImpl() = 0;
		virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
		virtual TrackMetaDataPtr createTrackMetaData() = 0;
		virtual TrackId createMasterTrack();
		compiler::IDefinitionsServerPtr definitionsServer_;

	private:
		WarningHandler _warningHandler;
		double masterTempo_ = com::DefaultTempo;
		VoicingStrategyPtr defaultVoiceStrategy_;
		SheetTemplates currentSheetTemplates_;
		TrackId trackId_ = INVALID_TRACK_ID,
				chordTrack_ = INVALID_TRACK_ID,
				masterTrackId_ = INVALID_TRACK_ID;
		VoiceId voiceId_ = INVALID_VOICE_ID, chordVoice_ = INVALID_VOICE_ID;
		VoiceMetaDataMap voiceMetaDataMap_;
		TrackMetaDataMap trackMetaDataMap_;
		ICompilerVisitorPtr _compilerVisitor;
	};
}
