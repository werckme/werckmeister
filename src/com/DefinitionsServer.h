#ifndef SHEET_TEMPLATEDEF_SERVER_HPP
#define SHEET_TEMPLATEDEF_SERVER_HPP

#include <sheet/objects/ChordDef.h>
#include <sheet/SheetDef.h>
#include "IDefinitionsServer.h"
#include <sheet/Document.h>
#include <unordered_map>
#include <forward.hpp>
#include <compiler/IPreprocessor.h>

namespace com {

    class DefinitionsServer : public IDefinitionsServer {
	private:
    public:
		DefinitionsServer(sheet::DocumentPtr document)
			: document_(document) 
		{}
		typedef com::String SheetTemplateName;
		typedef com::String PartName;
		typedef sheet::SheetTemplate SheetTemplate;
		typedef std::unordered_map<SheetTemplateName, SheetTemplate> SheetTemplates;
        virtual ~DefinitionsServer() = default;
		virtual sheet::SheetTemplate getSheetTemplate(const com::String &name);
		virtual ConstChordValueType getChord(const com::String &name);
		virtual ConstPitchDefValueType getAlias(com::String alias);
		virtual sheet::PitchDef resolvePitch(const sheet::PitchDef &pitch);
		virtual com::String defaultSheetTemplateName() const { return "?"; }
	protected:
		SheetTemplates & sheetTemplates();
	private:
		void prepareTemplateDefinitions();
		SheetTemplate * findSheetTemplate(const com::String &sheetTemplateName);
		std::unique_ptr<SheetTemplates> sheetTemplates_;
		sheet::DocumentPtr document_;
    };

}

#endif