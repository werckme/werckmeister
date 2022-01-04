#pragma once

#include <documentModel/objects/ChordDef.h>
#include <documentModel/SheetDef.h>
#include "IDefinitionsServer.h"
#include <documentModel/Document.h>
#include <unordered_map>
#include <forward.hpp>
#include <compiler/IPreprocessor.h>

namespace compiler
{

	class DefinitionsServer : public IDefinitionsServer
	{
	private:
	public:
		DefinitionsServer(documentModel::DocumentPtr document)
			: document_(document)
		{
		}
		typedef com::String SheetTemplateName;
		typedef com::String PartName;
		typedef documentModel::SheetTemplate SheetTemplate;
		typedef std::unordered_map<SheetTemplateName, SheetTemplate> SheetTemplates;
		virtual ~DefinitionsServer() = default;
		virtual documentModel::SheetTemplate getSheetTemplate(const com::String &name);
		virtual ConstChordValueType getChord(const com::String &name);
		virtual ConstPitchDefValueType getAlias(com::String alias);
		virtual documentModel::PitchDef resolvePitch(const documentModel::PitchDef &pitch);
		virtual com::String defaultSheetTemplateName() const { return "?"; }

	protected:
		SheetTemplates &sheetTemplates();

	private:
		void prepareTemplateDefinitions();
		SheetTemplate *findSheetTemplate(const com::String &sheetTemplateName);
		std::unique_ptr<SheetTemplates> sheetTemplates_;
		documentModel::DocumentPtr document_;
	};

}
