#pragma once

#include <com/common.hpp>
#include "objects/ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include <memory>
#include <boost/bimap.hpp>
#include <memory>
#include "objects/ConductionSheetDef.h"
#include <list>

namespace documentModel
{
	class Document : public std::enable_shared_from_this<Document>
	{
	public:
		typedef std::unordered_map<com::String, ChordDef> ChordDefs;
		typedef std::unordered_map<com::String, PitchDef> PitchmapDefs;
		typedef com::String Path;
		typedef boost::bimap<Event::SourceId, Path> Sources;
		com::String path;
		Event::SourceId sourceId;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		PitchmapDefs pitchmapDefs;
		std::list<ConductionSheetDef> conductionSheets;
		Sources sources;
		Event::SourceId addSource(const Path &path);
		Path findSourcePath(Event::SourceId id) const;
		Event::SourceId findSourceId(const Path &path) const;
		static Event::SourceId getSourceId(const Path &path);

	protected:
		com::String getAbsolutePath(const com::String &path) const;
	};

}
