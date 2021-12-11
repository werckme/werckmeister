#pragma once

#include <com/common.hpp>
#include <vector>
#include "Event.h"
#include "ASheetObjectWithSourceInfo.h"
#include <documentModel/Argument.h>
#include "TrackConfig.h"
#include "Voice.h"

namespace documentModel {

	struct Track : ASheetObjectWithSourceInfo {
		typedef ASheetObjectWithSourceInfo Base;
		typedef std::vector<Voice> Voices;
		typedef std::vector<TrackConfig> TrackConfigs;
		TrackConfigs trackConfigs;
		Voices voices;
		com::Ticks maxLength() const;
	};

}

