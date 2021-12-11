#pragma once

#include <com/common.hpp>
#include <vector>
#include "ASheetObjectWithSourceInfo.h"
#include <documentModel/Argument.h>

namespace documentModel {

	struct TrackConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<documentModel::Argument> Args;
		com::String name;
		Args args;
	};

}

