#ifndef SHEET_TEMPLATEDEF_SERVER_HPP
#define SHEET_TEMPLATEDEF_SERVER_HPP

#include "ChordDef.h"
#include "SheetDef.h"
namespace sheet {

    class ISheetTemplateDefServer {
    public:
		typedef ChordDef* ChordValueType;
		typedef PitchDef* PitchDefValueType;
		typedef std::vector<const Track*> Tracks;
		struct SheetTemplate
		{
			SheetTemplate() = default;
			SheetTemplate(const fm::String &name) : name(name) {}
			Tracks tracks;
			bool empty() const { return tracks.empty(); }
			fm::String name;
			bool operator==(const SheetTemplate &b) const 
			{
				return name == b.name;
			}
			bool operator!=(const SheetTemplate &b)	const { return !(*this == b); }
		};
		
		typedef const ChordDef* ConstChordValueType;
		typedef const PitchDef* ConstPitchDefValueType;
        ~ISheetTemplateDefServer() = default;
		virtual SheetTemplate getSheetTemplate(const fm::String &name) = 0;
		virtual ConstChordValueType getChord(const fm::String &name) = 0;
		virtual ConstPitchDefValueType getAlias(fm::String alias) = 0;
    };

}

#endif