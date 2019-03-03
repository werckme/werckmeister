#ifndef SHEET_TRACK_H
#define SHEET_TRACK_H

#include <fm/common.hpp>
#include <vector>
#include "Event.h"
#include "ASheetObject.hpp"

namespace sheet {

	struct Voice : public ASheetObject {
		typedef std::vector<Event> Events;
		Events events;
	};

	struct TrackInfo : public ASheetObjectWithSourceInfo {
		typedef std::vector<fm::String> Args;
		fm::String name;
		Args args;
	};

	struct Track  {
		typedef ASheetObjectWithSourceInfo Base;
		typedef std::vector<Voice> Voices;
		typedef std::vector<TrackInfo> TrackInfos;
		TrackInfos trackInfos;
		Voices voices;
	};

}

#endif