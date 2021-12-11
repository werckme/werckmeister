#ifndef SHEET_DOCUMENT_CONFIG_H
#define SHEET_DOCUMENT_CONFIG_H

#include <com/common.hpp>
#include <vector>
#include "ASheetObject.h"
#include <sheet/Argument.h>

namespace sheet {

	struct DocumentConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<sheet::Argument> Args;
		com::String name;
		Args args;
	};

}

#endif