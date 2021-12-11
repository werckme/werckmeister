#ifndef DEGREEDEF_H
#define DEGREEDEF_H

#include "Interval.h"

namespace documentModel {
	struct DegreeDef : public Interval {
		com::Pitch degree;
		bool operator <(const DegreeDef &b) const { return degree < b.degree; }
		static DegreeDef invalid() {
			DegreeDef res;
			res.value = INVALID_VALUE;
			return res;
		}
	};
}

#endif