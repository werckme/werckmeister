#ifndef SHEET_DOCUMENT_HPP
#define SHEET_DOCUMENT_HPP

#include <fm/common.hpp>
#include "objects/ChordDef.h"
#include "SheetDef.h"
#include "Pitchmap.h"
#include <unordered_map>
#include <fm/SheetTemplateDefServer.h>
#include <memory>
#include <boost/bimap.hpp>
#include <memory>

namespace sheet {
	class Document: public std::enable_shared_from_this<Document>, public ISheetTemplateDefServer {
	public:
		typedef ISheetTemplateDefServer::SheetTemplate SheetTemplateType;
		typedef std::unordered_map<fm::String, ChordDef> ChordDefs;
		typedef std::unordered_map<fm::String, PitchDef> PitchmapDefs;
		typedef fm::String SheetTemplateName;
		typedef fm::String PartName;
		typedef ISheetTemplateDefServer::SheetTemplate SheetTemplate;
		typedef std::unordered_map<SheetTemplateName, SheetTemplate> SheetTemplates;
		typedef fm::String Path;
		typedef boost::bimap<Event::SourceId, Path> Sources;
		fm::String path;
		Event::SourceId sourceId;
		SheetDef sheetDef;
		ChordDefs chordDefs;
		PitchmapDefs pitchmapDefs;
		Sources sources;
		SheetTemplateType getSheetTemplate(const fm::String &name) override;
		ISheetTemplateDefServer::ConstChordValueType getChord(const fm::String &name) override;
		ISheetTemplateDefServer::ConstPitchDefValueType getAlias(fm::String alias) override;
		SheetTemplates & sheetTemplates();
		Event::SourceId addSource(const Path &path);
		Path findSourcePath(Event::SourceId id) const;
		Event::SourceId findSourceId(const Path &path) const;
		static Event::SourceId getSourceId(const Path &path);
	protected:
		fm::String getAbsolutePath(const fm::String &path) const;
	private:
		SheetTemplateType * findSheetTemplate(const fm::String &sheetTemplateName);
		std::unique_ptr<SheetTemplates> sheetTemplates_;
		void createSheetTemplatesMap();
	};
}


#endif