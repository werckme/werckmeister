#ifndef TRACKCONFIG_H
#define TRACKCONFIG_H

#include <fm/common.hpp>
#include <vector>
#include "ASheetObjectWithSourceInfo.h"
#include "Argument.h"

namespace sheet {

	struct TrackConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<sheet::Argument> Args;
		fm::String name;
		Args args;
	};

}

#endif