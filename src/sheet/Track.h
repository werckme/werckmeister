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

	struct TrackInfo {
		typedef std::vector<fm::String> Args;
		fm::String name;
		Args args;
	};

	struct Track {
		typedef std::vector<Voice> Voices;
		typedef std::vector<TrackInfo> TrackInfos;
		TrackInfos trackInfos;
		Voices voices;
	};

}

#endif