#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "DocumentConfig.h"
#include "ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include "sheet/StyleDefServer.h"
#include <memory>

namespace sheet {
	class Document : public IStyleDefServer {
	public:
		typedef IStyleDefServer::ConstStyleValueType StyleType;
		typedef std::unordered_map<fm::String, ChordDef> ChordDefs;
		typedef std::unordered_map<fm::String, PitchDef> PitchmapDefs;
		typedef fm::String StyleName;
		typedef fm::String PartName;
		typedef std::vector<Track> Tracks;
		typedef std::unordered_map<PartName, Tracks> Parts;
		typedef std::unordered_map<StyleName, Parts> Styles;
		fm::String path;
		DocumentConfig documentConfig;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		PitchmapDefs pitchmapDefs;
		StyleType getStyle(const fm::String &filename, const fm::String &part = FM_STRING("?")) override;
		IStyleDefServer::ConstChordValueType getChord(const fm::String &name) override;
		IStyleDefServer::ConstPitchDefValueType getAlias(fm::String alias) override;
		/**
		 * creates an absolute path from a path relative to the document.
		 **/
		fm::String getAbsolutePath(const fm::String &path);
		const Styles & styles();
	private:
		Parts * findParts(const fm::String &styleName);
		Tracks * findTracks(const fm::String &partName, Parts &parts);
		std::unique_ptr<Styles> styles_;
		void createStylesMap();
	};
}


#endif