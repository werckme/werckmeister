#ifndef STYLEDEF_SERVER_HPP
#define STYLEDEF_SERVER_HPP

#include "ChordDef.h"
#include "SheetDef.h"
namespace sheet {

    class IStyleDefServer {
    public:
		typedef ChordDef* ChordValueType;
		typedef PitchDef* PitchDefValueType;
		typedef std::vector<const Track*> Tracks;
		struct Style
		{
			Style() = default;
			Style(const fm::String &name, const fm::String &part) : name(name), part(part) {}
			Tracks tracks;
			bool empty() const { return tracks.empty(); }
			fm::String name;
			fm::String part;
			bool operator==(const Style &b) const 
			{
				return name == b.name && part == b.part;
			}
			bool operator!=(const Style &b)	const { return !(*this == b); }
		};
		
		typedef const ChordDef* ConstChordValueType;
		typedef const PitchDef* ConstPitchDefValueType;
        ~IStyleDefServer() = default;
		virtual Style getStyle(const fm::String &name, const fm::String &part = FM_STRING("?")) = 0;
		virtual ConstChordValueType getChord(const fm::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(fm::String alias) = 0;
    };

}

#endif