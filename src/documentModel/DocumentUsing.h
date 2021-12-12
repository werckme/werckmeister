#pragma once

#include <vector>
#include <com/common.hpp>

namespace documentModel
{
	struct DocumentUsing
	{
		typedef com::String Using;
		typedef std::vector<Using> Usings;
		Usings usings;
	};
}
