#pragma once

#include <com/common.hpp>
#include "objects/Track.h"
#include "objects/Event.h"
#include <vector>
#include "objects/ASheetObject.h"
#include "objects/DocumentConfig.h"

namespace documentModel
{

	struct SheetDef
	{
		typedef com::String Using;
		typedef std::vector<Using> Usings;
		typedef std::vector<Track> Tracks;
		typedef std::vector<DocumentConfig> DocumentConfigs;
		Usings documentUsings;
		DocumentConfigs documentConfigs;
		Tracks tracks;
	};

}
