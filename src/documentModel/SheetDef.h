#pragma once

#include <com/common.hpp>
#include "objects/Track.h"
#include "objects/Event.h"
#include <vector>
#include "DocumentUsing.h"
#include "objects/ASheetObject.h"
#include "objects/DocumentConfig.h"
#include "objects/PhraseDef.h"

namespace documentModel
{

	struct SheetDef
	{
		typedef std::vector<Track> Tracks;
		typedef std::vector<DocumentConfig> DocumentConfigs;
		typedef std::vector<PhraseDef> PhraseDefs;
		DocumentUsing documentUsing;
		DocumentConfigs documentConfigs;
		PhraseDefs phraseDefs;
		Tracks tracks;
	};

}
