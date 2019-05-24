#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include "sheet/StyleDefServer.h"
#include <memory>
#include <boost/bimap.hpp>
#include <memory>

namespace sheet {
	class Document: public std::enable_shared_from_this<Document>, public IStyleDefServer {
	public:
		typedef IStyleDefServer::Style StyleType;
		typedef std::unordered_map<fm::String, ChordDef> ChordDefs;
		typedef std::unordered_map<fm::String, PitchDef> PitchmapDefs;
		typedef fm::String StyleName;
		typedef fm::String PartName;
		typedef IStyleDefServer::Style Style;
		typedef std::unordered_map<PartName, Style> Parts;
		typedef std::unordered_map<StyleName, Parts> Styles;
		typedef fm::String Path;
		typedef boost::bimap<Event::SourceId, Path> Sources;
		fm::String path;
		Event::SourceId sourceId;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		PitchmapDefs pitchmapDefs;
		Sources sources;
		StyleType getStyle(const fm::String &name, const fm::String &part = FM_STRING("?")) override;
		IStyleDefServer::ConstChordValueType getChord(const fm::String &name) override;
		IStyleDefServer::ConstPitchDefValueType getAlias(fm::String alias) override;
		Styles & styles();
		Event::SourceId addSource(const Path &path);
		Path findSourcePath(Event::SourceId id) const;
		Event::SourceId findSourceId(const Path &path) const;
		static Event::SourceId getSourceId(const Path &path);
	protected:
		fm::String getAbsolutePath(const fm::String &path) const;
	private:
		Parts * findParts(const fm::String &styleName);
		StyleType * findStyle(const fm::String &partName, Parts &parts);
		std::unique_ptr<Styles> styles_;
		void createStylesMap();
	};
}


#endif