#ifndef SHEET_TEMPLATEDEF_SERVER_HPP
#define SHEET_TEMPLATEDEF_SERVER_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/SheetDef.h>
#include "IDefinitionsServer.h"
#include <sheet/Document.h>
#include <unordered_map>

namespace fm {

    class DefinitionsServer : public IDefinitionsServer {
	private:
    public:
		typedef fm::String SheetTemplateName;
		typedef fm::String PartName;
		typedef sheet::SheetTemplate SheetTemplate;
		typedef std::unordered_map<SheetTemplateName, SheetTemplate> SheetTemplates;
        virtual ~DefinitionsServer() = default;
		virtual sheet::SheetTemplate getSheetTemplate(const fm::String &name);
		virtual ConstChordValueType getChord(const fm::String &name);
		virtual ConstPitchDefValueType getAlias(fm::String alias);
		virtual sheet::PitchDef resolvePitch(const sheet::PitchDef &pitch) const;		
	private:
		SheetTemplate * findSheetTemplate(const fm::String &sheetTemplateName);
		std::unique_ptr<SheetTemplates> sheetTemplates_;
		void createSheetTemplatesMap();
    };

}

#endif