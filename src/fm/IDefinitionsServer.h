#ifndef SHEET_ITEMPLATEDEF_SERVER_HPP
#define SHEET_ITEMPLATEDEF_SERVER_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/SheetDef.h>
#include <sheet/SheetTemplate.h>
#include <forward.hpp>
#include <memory>

namespace fm {
    class IDefinitionsServer {
    public:
		typedef sheet::ChordDef* ChordValueType;
		typedef sheet::PitchDef* PitchDefValueType;
		typedef std::vector<const sheet::Track*> Tracks;
		typedef const sheet::ChordDef* ConstChordValueType;
		typedef const sheet::PitchDef* ConstPitchDefValueType;
		virtual sheet::SheetTemplate getSheetTemplate(const fm::String &name) = 0;
		virtual ConstChordValueType getChord(const fm::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(fm::String alias) = 0;
		/**
		 * resolve pitches alias if exists
		 */		
		virtual sheet::PitchDef resolvePitch(const sheet::PitchDef &pitch) = 0;
    };
	typedef std::shared_ptr<IDefinitionsServer> IDefinitionsServerPtr;
}

#endif