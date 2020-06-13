#include "DefinitionsServer.h"
#include <sheet/Document.h>
#include <fm/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>

namespace fm {
    namespace {
        template<typename TContainer>
        auto _findByName(const fm::String &name, const TContainer &container)
        {
            typename TContainer::const_iterator it;
            if (name == FM_STRING("?")) {
                it = container.begin();
            } else {
                it = container.find(name);
            }
            return it;			
        }
    }
    sheet::SheetTemplate * DefinitionsServer::findSheetTemplate(const fm::String &sheetTemplateName)
	{
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		SheetTemplates::iterator it = sheetTemplates.find(sheetTemplateName);
		if (it == sheetTemplates.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	DefinitionsServer::SheetTemplates & DefinitionsServer::sheetTemplates()
	{
		return *sheetTemplates_;
	}

	sheet::SheetTemplate DefinitionsServer::getSheetTemplate(const fm::String &name)
	{
		const SheetTemplates &sheetTemplates = this->sheetTemplates();
		// find sheetTemplate by name
		SheetTemplates::const_iterator it = _findByName(name, sheetTemplates);
		if (it == sheetTemplates.end()) {
			return sheet::SheetTemplate();
		}
		return it->second;
	}

	IDefinitionsServer::ConstChordValueType DefinitionsServer::getChord(const fm::String &name)
	{
		sheet::Document::ChordDefs::const_iterator it;
		if (name == FM_STRING("?")) {
			it = document_->chordDefs.begin();
		}
		else {
			it = document_->chordDefs.find(name);
		}
		if (it == document_->chordDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	IDefinitionsServer::ConstPitchDefValueType DefinitionsServer::getAlias(fm::String alias)
	{
		sheet::Document::PitchmapDefs::const_iterator it;
		it = document_->pitchmapDefs.find(alias);
		
		if (it == document_->pitchmapDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	sheet::PitchDef DefinitionsServer::resolvePitch(const sheet::PitchDef &pitch)
	{
		if (pitch.alias.empty()) {
			return pitch;
		}
		const sheet::PitchDef *result = getAlias(pitch.alias);
		if (result == nullptr) {
			FM_THROW(Exception, "could not resolve alias: " + pitch.alias);
		}
		return *result;
	}
}
