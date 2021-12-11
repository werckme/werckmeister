#ifndef SHEET_ITEMPLATEDEF_SERVER_HPP
#define SHEET_ITEMPLATEDEF_SERVER_HPP

#include <documentModel/objects/ChordDef.h>
#include <documentModel/SheetDef.h>
#include <documentModel/SheetTemplate.h>
#include <forward.hpp>
#include <memory>

namespace com {
    class IDefinitionsServer {
    public:
		typedef documentModel::ChordDef* ChordValueType;
		typedef documentModel::PitchDef* PitchDefValueType;
		typedef std::vector<const documentModel::Track*> Tracks;
		typedef const documentModel::ChordDef* ConstChordValueType;
		typedef const documentModel::PitchDef* ConstPitchDefValueType;
		virtual documentModel::SheetTemplate getSheetTemplate(const com::String &name) = 0;
		virtual ConstChordValueType getChord(const com::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(com::String alias) = 0;
		/**
		 * resolve pitches alias if exists
		 */		
		virtual documentModel::PitchDef resolvePitch(const documentModel::PitchDef &pitch) = 0;
		virtual com::String defaultSheetTemplateName() const = 0;
    };
	typedef std::shared_ptr<IDefinitionsServer> IDefinitionsServerPtr;
}

#endif