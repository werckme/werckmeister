#pragma once

#include <com/common.hpp>
#include <vector>
#include "ASheetObject.h"
#include <documentModel/Argument.h>

namespace documentModel
{
	struct Event;
	struct DocumentConfig : public ASheetObjectWithSourceInfo
	{
		enum Type 
		{
			TypeConfigDef,
			TypePhraseDef,
		};
		typedef std::vector<documentModel::Argument> Args;
		typedef std::vector<Event> Events;
		Type type = TypeConfigDef;
		com::String name;
		Args args;
		Events events;
	};

}
