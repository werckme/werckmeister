#ifndef TRACKCONFIG_H
#define TRACKCONFIG_H

#include <com/common.hpp>
#include <vector>
#include "ASheetObjectWithSourceInfo.h"
#include <sheet/Argument.h>

namespace sheet {

	struct TrackConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<sheet::Argument> Args;
		com::String name;
		Args args;
	};

}

#endif