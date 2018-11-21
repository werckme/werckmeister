#ifndef CHORD_DEF_H
#define CHORD_DEF_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <set>
#include "Interval.h"

namespace sheet {

    struct ChordDef {
        typedef std::set<ChordOption> Intervals;
        fm::String name;
		Intervals intervals;
		const ChordOption * getIntervalBy(fm::Pitch degree) const;
	};

}

#endif