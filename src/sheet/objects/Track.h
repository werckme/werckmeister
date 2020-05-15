#ifndef SHEET_TRACK_H
#define SHEET_TRACK_H

#include <fm/common.hpp>
#include <vector>
#include "Event.h"
#include "ASheetObjectWithSourceInfo.h"
#include "Argument.h"
#include "TrackConfig.h"

namespace sheet {

	struct Track : ASheetObjectWithSourceInfo {
		typedef ASheetObjectWithSourceInfo Base;
		typedef std::vector<Voice> Voices;
		typedef std::vector<TrackConfig> TrackConfigs;
		TrackConfigs trackConfigs;
		Voices voices;
	};

}

#endif