#ifndef SHEET_DOCUMENT_CONFIG_H
#define SHEET_DOCUMENT_CONFIG_H

#include <com/common.hpp>
#include <vector>
#include "ASheetObject.hpp"

namespace sheet {

	struct DocumentConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<com::String> Args;
		com::String name;
		Args args;
	};

}

#endif