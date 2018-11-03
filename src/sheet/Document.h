#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "DocumentConfig.h"
#include "StyleDef.h"
#include "ChordDef.h"
#include "SheetDef.h"
#include <unordered_map>

namespace sheet {
	class Document {
	public:
		typedef std::unordered_map<fm::String, ChordDef::Intervals> ChordDefs;
		typedef std::unordered_map<fm::String, StyleDef> StyleDefs;
		fm::String path;
		DocumentConfig documentConfig;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		StyleDefs styleDefs;
	};
}


#endif