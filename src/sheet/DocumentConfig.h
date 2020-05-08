#ifndef SHEET_DOCUMENT_CONFIG_H
#define SHEET_DOCUMENT_CONFIG_H

#include <fm/common.hpp>
#include <vector>
#include "ASheetObject.hpp"

namespace sheet {

	struct DocumentConfig : public ASheetObjectWithSourceInfo {
		typedef std::vector<fm::String> Args;
		fm::String name;
		Args args;
	};

}

#endif