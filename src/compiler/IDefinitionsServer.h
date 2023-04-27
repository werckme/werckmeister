#pragma once

#include <documentModel/objects/ChordDef.h>
#include <documentModel/SheetDef.h>
#include <documentModel/SheetTemplate.h>
#include <forward.hpp>
#include <memory>

namespace compiler
{
	class IDefinitionsServer
	{
	public:
		typedef documentModel::ChordDef *ChordValueType;
		typedef documentModel::PitchDef *PitchDefValueType;
		typedef std::vector<const documentModel::Track *> Tracks;
		typedef const documentModel::ChordDef* ConstChordValueType;
		typedef const documentModel::PitchDef* ConstPitchDefValueType;
		typedef documentModel::DocumentConfig::Events Phrase;
		typedef const Phrase* ConstPhraseDefValueType;
		virtual documentModel::SheetTemplate getSheetTemplate(const com::String &name) = 0;
		virtual ConstChordValueType getChord(const com::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(const com::String &alias) = 0;
		virtual ConstPhraseDefValueType getPhrase(const com::String &name) = 0;
		/**
		 * resolve pitches alias if exists
		 */
		virtual documentModel::PitchDef resolvePitch(const documentModel::PitchDef &pitch) = 0;
		virtual com::String defaultSheetTemplateName() const = 0;
	};
	typedef std::shared_ptr<IDefinitionsServer> IDefinitionsServerPtr;
}
