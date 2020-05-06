#ifndef SHEET_SHEETDEF_H
#define SHEET_SHEETDEF_H

#include <fm/common.hpp>
#include "Track.h"
#include "Event.h"
#include <vector>
#include "DocumentUsing.h"
#include "ASheetObject.hpp"
#include "Argument.h"

namespace sheet {

	struct DocumentConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<sheet::Argument> Args;
		fm::String name;
		Args args;
	};

	struct SheetDef {
		typedef std::vector<Track> Tracks;
		typedef std::vector<DocumentConfig> DocumentConfigs;
		DocumentUsing documentUsing;
		DocumentConfigs documentConfigs;
		Tracks tracks;
	};

}

#endif