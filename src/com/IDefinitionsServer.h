#ifndef SHEET_ITEMPLATEDEF_SERVER_HPP
#define SHEET_ITEMPLATEDEF_SERVER_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/SheetDef.h>
#include <sheet/SheetTemplate.h>
#include <forward.hpp>
#include <memory>

namespace com {
    class IDefinitionsServer {
    public:
		typedef sheet::ChordDef* ChordValueType;
		typedef sheet::PitchDef* PitchDefValueType;
		typedef std::vector<const sheet::Track*> Tracks;
		typedef const sheet::ChordDef* ConstChordValueType;
		typedef const sheet::PitchDef* ConstPitchDefValueType;
		virtual sheet::SheetTemplate getSheetTemplate(const com::String &name) = 0;
		virtual ConstChordValueType getChord(const com::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(com::String alias) = 0;
		/**
		 * resolve pitches alias if exists
		 */		
		virtual sheet::PitchDef resolvePitch(const sheet::PitchDef &pitch) = 0;
		virtual com::String defaultSheetTemplateName() const = 0;
    };
	typedef std::shared_ptr<IDefinitionsServer> IDefinitionsServerPtr;
}

#endif