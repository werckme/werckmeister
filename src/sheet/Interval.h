#ifndef INTERVAL_H
#define INTERVAL_H

#include <fm/units.hpp>

namespace sheet {
    struct Interval {
        int value;
		bool operator <(const Interval &b) const { return value < b.value; }
    };

	struct ChordOption : public Interval {
		fm::Pitch degree;
		bool operator <(const ChordOption &b) const { return degree < b.degree; }
	};
}

#endif