#ifndef PITCHMAP_DEF_H
#define PITCHMAP_DEF_H

#include <com/common.hpp>
#include <com/units.hpp>
#include <map>
#include "objects/Interval.h"
#include "objects/Event.h"

namespace sheet {

    struct Pitchmap {
        com::String name;
		PitchDef pitch;
	};

}

#endif