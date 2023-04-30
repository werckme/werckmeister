#pragma once

#include <documentModel/objects/ChordDef.h>
#include <documentModel/SheetDef.h>
#include "IDefinitionsServer.h"
#include <documentModel/Document.h>
#include <unordered_map>
#include <forward.hpp>
#include <compiler/IPreprocessor.h>
#include <compiler/ICompilerVisitor.h>

namespace compiler
{

	class DefinitionsServer : public IDefinitionsServer
	{
	private:
	public:
		DefinitionsServer(documentModel::DocumentPtr document, ICompilerVisitorPtr compilerVisitor)
			: document_(document), compilerVisitor_(compilerVisitor) 
		{
		}
		typedef com::String SheetTemplateName;
		typedef com::String PhraseDefName;
		typedef com::String PartName;
		typedef documentModel::SheetTemplate SheetTemplate;
		typedef documentModel::DocumentConfig PhraseDef;
		typedef std::unordered_map<SheetTemplateName, SheetTemplate> SheetTemplates;
		typedef std::unordered_map<PhraseDefName, PhraseInfo> PhraseDefs;
		virtual ~DefinitionsServer() = default;
		virtual documentModel::SheetTemplate getSheetTemplate(const com::String &name) override;
		virtual ConstChordValueType getChord(const com::String &name) override;
		virtual ConstPitchDefValueType getAlias(const com::String &alias) override;
		virtual ConstPhraseDefValueType getPhrase(const com::String &name) override;
		virtual documentModel::PitchDef resolvePitch(const documentModel::PitchDef &pitch);
		virtual com::String defaultSheetTemplateName() const { return "?"; }
		virtual void degreeToAbsoluteNote(IContextPtr context, const Event &chordEvent, const Event &degreeEvent, Event &outTarget) override;

	protected:
		SheetTemplates &sheetTemplates();
		PhraseDefs &phraseDefs();

	private:
		void prepareTemplateDefinitions();
		void preparePhraseDefinitions();
		SheetTemplate *findSheetTemplate(const com::String &sheetTemplateName);
		std::unique_ptr<SheetTemplates> sheetTemplates_;
		std::unique_ptr<PhraseDefs> phraseDefs_;
		documentModel::DocumentPtr document_;
		ICompilerVisitorPtr compilerVisitor_;
	};

}
