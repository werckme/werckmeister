#ifndef SHEET_SHEETDEF_H
#define SHEET_SHEETDEF_H

#include <fm/common.hpp>
#include "Track.h"
#include <vector>

namespace sheet {

	struct SheetDef {
		typedef std::vector<Track> Tracks;
		Tracks tracks;
	};

}

#endif