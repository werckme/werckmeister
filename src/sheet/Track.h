#ifndef SHEET_TRACK_H
#define SHEET_TRACK_H

#include <fm/common.hpp>
#include <vector>
#include "Event.h"

namespace sheet {

	struct Voice {
		typedef std::vector<Event> Events;
		Events events;
	};

	struct Track {
		typedef std::vector<Voice> Voices;
		Voices voices;
	};

}

#endif