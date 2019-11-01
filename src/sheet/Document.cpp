#include "Document.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <sheet/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>
#include <functional>
#include <fm/werckmeister.hpp>
#include <compiler/preprocessor.h>

namespace sheet {
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

	fm::String Document::getAbsolutePath(const fm::String &path) const
	{
		sheet::ConstDocumentPtr thisPtr = shared_from_this();
		return fm::getWerckmeister().resolvePath(path, thisPtr);
	}

	Document::SheetTemplate * Document::findSheetTemplate(const fm::String &sheetTemplateName)
	{
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		SheetTemplates::iterator it = sheetTemplates.find(sheetTemplateName);
		if (it == sheetTemplates.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	void Document::createSheetTemplatesMap()
	{
		sheetTemplates_ = std::make_unique<SheetTemplates>();
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		compiler::Preprocessor preprocessor;
		for(auto &track : this->sheetDef.tracks) {
			fm::String type = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_TYPE, track.trackInfos);
			if (type != SHEET_META__TRACK_META_VALUE_TYPE_SHEET_TEMPLATE) {
				continue;
			}
			fm::String sheetTemplateName = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_NAME, track.trackInfos);
			if (sheetTemplateName.empty()) {
				FM_THROW(compiler::Exception, "missing 'name' for sheetTemplate track");
			}					
			auto sheetTemplate = findSheetTemplate(sheetTemplateName);
			if (sheetTemplate == nullptr) {
				sheetTemplates[sheetTemplateName] = SheetTemplate(sheetTemplateName);
				sheetTemplate = &(sheetTemplates[sheetTemplateName]);
			}
			preprocessor.process(track);
			sheetTemplate->tracks.push_back(&track);
		}
	}

	Document::SheetTemplates & Document::sheetTemplates()
	{
		if (!sheetTemplates_) {
			createSheetTemplatesMap();
		}
		return *sheetTemplates_;
	}

	Document::SheetTemplateType Document::getSheetTemplate(const fm::String &name)
	{
		const SheetTemplates &sheetTemplates = this->sheetTemplates();
		// find sheetTemplate by name
		SheetTemplates::const_iterator it = _findByName(name, sheetTemplates);
		if (it == sheetTemplates.end()) {
			return SheetTemplateType();
		}
		return it->second;
	}

	ISheetTemplateDefServer::ConstChordValueType Document::getChord(const fm::String &name)
	{
		ChordDefs::const_iterator it;
		if (name == FM_STRING("?")) {
			it = chordDefs.begin();
		}
		else {
			it = chordDefs.find(name);
		}
		if (it == chordDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	ISheetTemplateDefServer::ConstPitchDefValueType Document::getAlias(fm::String alias)
	{
		PitchmapDefs::const_iterator it;
		it = pitchmapDefs.find(alias);
		
		if (it == pitchmapDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	Event::SourceId Document::addSource(const Path &_path)
	{
		auto path = getAbsolutePath(_path);
		auto sourceId = findSourceId(path);
		if (sourceId != Event::UndefinedSource) {
			return sourceId;
		}
		sourceId = getSourceId(path);
		sources.insert({sourceId, path});
		return sourceId;
	}

	Document::Path Document::findSourcePath(Event::SourceId id) const
	{
		auto it = sources.left.find(id);
		if (it == sources.left.end()) {
			return Document::Path();
		}
		return it->second;
	}

	Event::SourceId Document::findSourceId(const Path &path) const
	{
		auto it = sources.right.find(getAbsolutePath(path));
		if (it == sources.right.end()) {
			return Event::UndefinedSource;
		}
		return it->second;
	}

	Event::SourceId Document::getSourceId(const Path &path)
	{
		return std::hash<Path>{}(path);
	}
}