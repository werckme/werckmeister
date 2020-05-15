#ifndef PITCHMAP_DEF_H
#define PITCHMAP_DEF_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <map>
#include "objects/Interval.h"
#include "objects/Event.h"

namespace sheet {

    struct Pitchmap {
        fm::String name;
		PitchDef pitch;
	};

}

#endif