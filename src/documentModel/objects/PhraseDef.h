#pragma once

#include <com/common.hpp>
#include <com/units.hpp>
#include <vector>
#include "ASheetObjectWithSourceInfo.h"

namespace documentModel
{
    struct Event;
    struct PhraseDef : public ASheetObjectWithSourceInfo
    {
        typedef std::vector<Event> Events;
        com::String name;
        Events events;
    };
}
