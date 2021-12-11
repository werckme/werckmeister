#ifndef SHEET_TEMPLATEDEF_HPP
#define SHEET_TEMPLATEDEF_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/SheetDef.h>
#include <vector>
#include <forward.hpp>

namespace sheet {

    struct SheetTemplate
    {
        typedef std::vector<const Track*> Tracks;
        SheetTemplate() = default;
        SheetTemplate(const com::String &name) : name(name) {}
        Tracks tracks;
        bool empty() const { return tracks.empty(); }
        com::String name;
        bool operator==(const SheetTemplate &b) const 
        {
            return name == b.name;
        }
        bool operator!=(const SheetTemplate &b)	const { return !(*this == b); }
        bool isFill = false;
        com::Ticks ignoreUnitlPosition = -1.0;
        com::Ticks maxLength() const;
    };

}

#endif