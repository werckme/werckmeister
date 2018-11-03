#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "DocumentConfig.h"
#include "StyleDef.h"
#include "ChordDef.h"
#include "SheetDef.h"

namespace sheet {
	class Document {
	public:
		DocumentConfig documentConfig;
		SheetDef sheetDef;
	};
}


#endif