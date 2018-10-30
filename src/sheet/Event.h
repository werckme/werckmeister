#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <vector>

namespace sheet {
	struct Event {
		enum {
			NoDuration = 0,
			NoPitch = -1,
		};
		enum Type { 
			Unknown,
			Rest,
			Degree, 
			Absolute, 
			EOB, // End of Bar aka. Bar Line
			Meta
		};
		typedef int Pitch;
		typedef fm::Ticks Duration;
		Pitch pitch = NoPitch;
		Type type = Unknown;
		Duration duration = NoDuration;
	};
}

#endif