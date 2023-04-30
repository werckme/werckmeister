#include "DefinitionsServer.h"
#include <documentModel/Document.h>
#include <com/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>
#include <numeric>
#include <compiler/context/IContext.h>

namespace compiler
{
	namespace
	{
		template <typename TContainer>
		auto _findByName(const com::String &name, const TContainer &container)
		{
			typename TContainer::const_iterator it;
			if (name == FM_STRING("?"))
			{
				it = container.begin();
			}
			else
			{
				it = container.find(name);
			}
			return it;
		}
	}
	documentModel::SheetTemplate *DefinitionsServer::findSheetTemplate(const com::String &sheetTemplateName)
	{
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		SheetTemplates::iterator it = sheetTemplates.find(sheetTemplateName);
		if (it == sheetTemplates.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	DefinitionsServer::SheetTemplates &DefinitionsServer::sheetTemplates()
	{
		if (!sheetTemplates_)
		{
			prepareTemplateDefinitions();
		}
		return *sheetTemplates_;
	}

	DefinitionsServer::PhraseDefs &DefinitionsServer::phraseDefs()
	{
		if (!phraseDefs_)
		{
			preparePhraseDefinitions();
		}
		return *phraseDefs_;
	}

	documentModel::SheetTemplate DefinitionsServer::getSheetTemplate(const com::String &name)
	{
		const SheetTemplates &sheetTemplates = this->sheetTemplates();
		// find sheetTemplate by name
		SheetTemplates::const_iterator it = _findByName(name, sheetTemplates);
		if (it == sheetTemplates.end())
		{
			return documentModel::SheetTemplate();
		}
		return it->second;
	}

	IDefinitionsServer::ConstChordValueType DefinitionsServer::getChord(const com::String &name)
	{
		documentModel::Document::ChordDefs::const_iterator it;
		if (name == FM_STRING("?"))
		{
			it = document_->chordDefs.begin();
		}
		else
		{
			it = document_->chordDefs.find(name);
		}
		if (it == document_->chordDefs.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	IDefinitionsServer::ConstPitchDefValueType DefinitionsServer::getAlias(const com::String &alias)
	{
		documentModel::Document::PitchmapDefs::const_iterator it;
		it = document_->pitchmapDefs.find(alias);

		if (it == document_->pitchmapDefs.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	IDefinitionsServer::ConstPhraseDefValueType DefinitionsServer::getPhrase(const com::String &name)
	{
		const PhraseDefs &phraseDef = this->phraseDefs();
		// find phrase by name
		PhraseDefs::const_iterator it = _findByName(name, phraseDef);
		if (it == phraseDef.end())
		{
			return ConstPhraseDefValueType();
		}
		return it->second;
	}

	documentModel::PitchDef DefinitionsServer::resolvePitch(const documentModel::PitchDef &pitch)
	{
		if (pitch.alias.empty())
		{
			return pitch;
		}
		const documentModel::PitchDef *result = getAlias(pitch.alias);
		if (result == nullptr)
		{
			FM_THROW(compiler::Exception, "could not resolve alias: " + pitch.alias);
		}
		documentModel::PitchDef resultCopy = *result;
		resultCopy.alias = pitch.alias;
		return resultCopy;
	}

	void DefinitionsServer::prepareTemplateDefinitions()
	{
		sheetTemplates_ = std::make_unique<SheetTemplates>();
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		for (auto &track : this->document_->sheetDef.tracks)
		{
			try
			{
				com::String type = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
				if (type != SHEET_META__TRACK_META_VALUE_TYPE_SHEET_TEMPLATE)
				{
					continue;
				}
				com::String sheetTemplateName = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_NAME, track.trackConfigs).value;
				if (sheetTemplateName.empty())
				{
					FM_THROW(compiler::Exception, "missing 'name' for sheetTemplate track");
				}
				auto sheetTemplate = findSheetTemplate(sheetTemplateName);
				if (sheetTemplate == nullptr)
				{
					sheetTemplates[sheetTemplateName] = SheetTemplate(sheetTemplateName);
					sheetTemplate = &(sheetTemplates[sheetTemplateName]);
				}
				sheetTemplate->tracks.push_back(&track);
			}
			catch (const com::Exception &ex)
			{
				ex << compiler::ex_sheet_source_info(track);
				throw;
			}
		}
	}

	void DefinitionsServer::preparePhraseDefinitions()
	{
		phraseDefs_ = std::make_unique<PhraseDefs>();
		for (auto &documentConfig : this->document_->sheetDef.documentConfigs)
		{
			if (documentConfig.type != documentModel::DocumentConfig::TypePhraseDef) 
			{
				continue;
			}
			try
			{
				PhraseInfo info;
				info.events = &documentConfig.events;
				for(const auto &event : *info.events) 
				{
					info.duration += event.duration;
				}
				phraseDefs_->emplace(std::make_pair(documentConfig.name, info));	
			}
			catch (const com::Exception &ex)
			{
				ex << compiler::ex_sheet_source_info(documentConfig);
				throw;
			}
		}
	}

	void DefinitionsServer::degreeToAbsoluteNote(IContextPtr ctx, const Event &chordEvent, const Event &degreeEvent, Event &outTarget)
	{
		outTarget = degreeEvent;
		if (degreeEvent.type == Event::Group)
		{
			outTarget.type = Event::Group;
			size_t index = 0;
			for (const auto &groupedDegreeEvent : degreeEvent.eventGroup)
			{
				degreeToAbsoluteNote(ctx, chordEvent, groupedDegreeEvent, outTarget.eventGroup[index++]);
			}
			return;
		}
		if (!degreeEvent.isRelativeDegree())
		{
			return;
		}
		auto chordDef = getChord(chordEvent.chordDefName());
		if (chordDef == nullptr)
		{
			FM_THROW(Exception, "chord not found: " + chordEvent.stringValue);
		}
		auto voicingStratgy = ctx->currentVoicingStrategy();
		auto pitches = voicingStratgy->get(chordEvent, *chordDef, degreeEvent.pitches, ctx->getTimeInfo());
		compilerVisitor_->visitDegree(chordEvent, *chordDef, degreeEvent);
		outTarget.type = Event::Note;
		outTarget.isTied(degreeEvent.isTied());
		outTarget.pitches.swap(pitches);
	}
}
