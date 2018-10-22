#ifndef SHEET_TRACK_H
#define SHEET_TRACK_H

#include <fm/common.hpp>
#include <vector>
#include "Event.h"

namespace sheet {

	struct Voice {
		typedef std::vector<events::AEvent> Events;
	};

	struct Track {
		typedef std::vector<Voice> Voices;
	};

}

#endif