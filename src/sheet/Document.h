#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "objects/ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include <memory>
#include <boost/bimap.hpp>
#include <memory>
#include "objects/ConductionSheetDef.h"

namespace sheet {
	class Document: public std::enable_shared_from_this<Document> {
	public:
		typedef std::unordered_map<fm::String, ChordDef> ChordDefs;
		typedef std::unordered_map<fm::String, PitchDef> PitchmapDefs;
		typedef std::vector<ConductionSheetDef> StyleDefs;
		typedef fm::String Path;
		typedef boost::bimap<Event::SourceId, Path> Sources;
		fm::String path;
		Event::SourceId sourceId;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		PitchmapDefs pitchmapDefs;
		StyleDefs styleDefs;
		Sources sources;
		Event::SourceId addSource(const Path &path);
		Path findSourcePath(Event::SourceId id) const;
		Event::SourceId findSourceId(const Path &path) const;
		static Event::SourceId getSourceId(const Path &path);
	protected:
		fm::String getAbsolutePath(const fm::String &path) const;
	};
	
}


#endif