#ifndef DEGREEDEF_H
#define DEGREEDEF_H

#include "Interval.h"

namespace sheet {
	struct DegreeDef : public Interval {
		fm::Pitch degree;
		bool operator <(const DegreeDef &b) const { return degree < b.degree; }
		static DegreeDef invalid() {
			DegreeDef res;
			res.value = INVALID_VALUE;
			return res;
		}
	};
}

#endif