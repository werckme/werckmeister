#ifndef INTERVAL_H
#define INTERVAL_H

#include <fm/units.hpp>
#include <climits>
#include "ASheetObject.hpp"

namespace sheet {
    struct Interval : public ASheetObject {
		enum { INVALID_VALUE = INT_MAX };
        int value;
		bool operator <(const Interval &b) const { return value < b.value; }
		bool valid() const { return value != INVALID_VALUE; }
    };

	struct ChordOption : public Interval {
		fm::Pitch degree;
		bool operator <(const ChordOption &b) const { return degree < b.degree; }
		static ChordOption invalid() {
			ChordOption res;
			res.value = INVALID_VALUE;
			return res;
		}
	};
}

#endif