#pragma once

#include <com/common.hpp>
#include <vector>
#include <documentModel/objects/ASheetObjectWithSourceInfo.h>

namespace documentModel
{

	struct DocumentConfig : public ASheetObjectWithSourceInfo
	{
		typedef std::vector<com::String> Args;
		com::String name;
		Args args;
	};

}
