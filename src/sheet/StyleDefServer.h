#ifndef STYLEDEF_SERVER_HPP
#define STYLEDEF_SERVER_HPP

#include "ChordDef.h"
#include "SheetDef.h"
namespace sheet {

    class IStyleDefServer {
    public:
		typedef SheetDef::Tracks* StyleValueType;
		typedef ChordDef* ChordValueType;
		typedef PitchDef* PitchDefValueType;
		typedef const SheetDef::Tracks* ConstStyleValueType;
		typedef const ChordDef* ConstChordValueType;
		typedef const PitchDef* ConstPitchDefValueType;
        ~IStyleDefServer() = default;
		virtual ConstStyleValueType getStyle(const fm::String &filename, const fm::String &sectionName = FM_STRING("?")) const = 0;
		virtual ConstChordValueType getChord(const fm::String &name) const = 0;
		virtual ConstPitchDefValueType getAlias(fm::String alias) const = 0;
    };

}

#endif