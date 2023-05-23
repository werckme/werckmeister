#pragma once

#include <documentModel/objects/ChordDef.h>
#include <documentModel/SheetDef.h>
#include <documentModel/SheetTemplate.h>
#include <compiler/voicings/VoicingStrategy.h>
#include <forward.hpp>
#include <memory>
#include <vector>
#include "timeInfo.h"

namespace documentModel 
{
	struct Event;
}

namespace compiler
{
	class IContext;
	typedef std::shared_ptr<IContext> IContextPtr;
	struct PhraseInfo 
	{
		typedef documentModel::DocumentConfig::Events PhraseEvents;
		const PhraseEvents *events = nullptr;
		com::Ticks duration = 0;
	};
	class IDefinitionsServer
	{
	public:
		typedef documentModel::Event Event;
		typedef documentModel::ChordDef *ChordValueType;
		typedef documentModel::PitchDef *PitchDefValueType;
		typedef std::vector<const documentModel::Track *> Tracks;
		typedef const documentModel::ChordDef* ConstChordValueType;
		typedef const documentModel::PitchDef* ConstPitchDefValueType;
		typedef PhraseInfo ConstPhraseDefValueType;
		virtual documentModel::SheetTemplate getSheetTemplate(const com::String &name) = 0;
		virtual ConstChordValueType getChord(const com::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(const com::String &alias) = 0;
		virtual ConstPhraseDefValueType getPhrase(const com::String &name) = 0;
		/**
		 * resolve pitches alias if exists
		 */
		virtual documentModel::PitchDef resolvePitch(const documentModel::PitchDef &pitch) = 0;
		virtual com::String defaultSheetTemplateName() const = 0;
		virtual void degreeToAbsoluteNote(IContextPtr context, const Event &chordEvent, const Event &degreeEvent, Event &outEvent, bool throwIfChordNotFound = true) = 0;
		virtual void degreeToAbsoluteNote(const VoicingStrategies &voicingStrategies, const TimeInfo &timeInfo, const Event &chordEvent, const Event &degreeEvent, Event &outEvent, bool throwIfChordNotFound = true, bool visitVisitors = true) = 0;
	};
	typedef std::shared_ptr<IDefinitionsServer> IDefinitionsServerPtr;
}
