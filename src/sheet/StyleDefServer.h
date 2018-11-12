#ifndef STYLEDEF_SERVER_HPP
#define STYLEDEF_SERVER_HPP

#include "StyleDef.h"
#include "ChordDef.h"

namespace sheet {

    class IStyleDefServer {
    public:
        ~IStyleDefServer() = default;
		virtual const StyleDef * getStyle(const fm::String &name) const = 0;
		virtual const ChordDef::Intervals * getChord(const fm::String &name) const = 0;
    };

}

#endif