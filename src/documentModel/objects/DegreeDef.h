#pragma once

#include "Interval.h"

namespace documentModel
{
	struct DegreeDef : public Interval
	{
		DegreeDef() {}
		DegreeDef(com::Pitch degree, int semitones, bool isAdjunct) 
		{
			this->degree = degree;
			Interval::value = semitones;
			this->isAdjunct = isAdjunct;
		}
		// Akkordfremd
		bool isAdjunct = false;
		com::Pitch degree;
		bool operator<(const DegreeDef &b) const { return degree < b.degree; }
		static DegreeDef invalid()
		{
			DegreeDef res;
			res.value = INVALID_VALUE;
			return res;
		}
	};
}
