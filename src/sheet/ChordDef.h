#ifndef CHORD_DEF_H
#define CHORD_DEF_H

#include <fm/common.hpp>
#include <set>
#include "Interval.h"

namespace sheet {

    struct ChordDef {
        typedef std::set<Interval> Intervals;
        fm::String name;
		Intervals intervals;
	};

}

#endif