#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "DocumentConfig.h"
#include "StyleDef.h"
#include "ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include "sheet/StyleDefServer.h"

namespace sheet {
	class Document : public IStyleDefServer {
	public:
		typedef std::unordered_map<fm::String, ChordDef> ChordDefs;
		typedef std::unordered_map<fm::String, StyleDef> StyleDefs;
		typedef std::unordered_map<fm::String, PitchDef> PitchmapDefs;
		fm::String path;
		DocumentConfig documentConfig;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		StyleDefs styleDefs;
		PitchmapDefs pitchmapDefs;
		IStyleDefServer::ConstStyleValueType getStyle(const fm::String &filename, const fm::String &sectionName = FM_STRING("?")) const override;
		IStyleDefServer::ConstChordValueType getChord(const fm::String &name) const override;
		IStyleDefServer::ConstPitchDefValueType getAlias(fm::String alias) const override;
	};
}


#endif