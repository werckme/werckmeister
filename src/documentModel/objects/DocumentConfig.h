#pragma once

#include <com/common.hpp>
#include <vector>
#include "ASheetObject.h"
#include <documentModel/Argument.h>

namespace documentModel
{

	struct DocumentConfig : public ASheetObjectWithSourceInfo
	{
		typedef std::vector<documentModel::Argument> Args;
		com::String name;
		Args args;
	};

}
