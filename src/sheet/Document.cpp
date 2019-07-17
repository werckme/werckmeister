#include "Document.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <sheet/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>
#include <functional>
#include <fm/werckmeister.hpp>

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

	Document::Style * Document::findStyle(const fm::String &styleName)
	{
		Styles &styles = *styles_;
		Styles::iterator it = styles.find(styleName);
		if (it == styles.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	void Document::createStylesMap()
	{
		styles_ = std::make_unique<Styles>();
		Styles &styles = *styles_;
		for(const auto &track : this->sheetDef.tracks) {
			fm::String type = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_TYPE, track.trackInfos);
			if (type != SHEET_META__TRACK_META_VALUE_TYPE_STYLE) {
				continue;
			}
			fm::String styleName = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_NAME, track.trackInfos);
			if (styleName.empty()) {
				FM_THROW(compiler::Exception, "missing 'name' for style track");
			}					
			auto style = findStyle(styleName);
			if (style == nullptr) {
				styles[styleName] = Style(styleName);
				style = &(styles[styleName]);
			}
			style->tracks.push_back(&track);
		}
	}

	Document::Styles & Document::styles()
	{
		if (!styles_) {
			createStylesMap();
		}
		return *styles_;
	}

	Document::StyleType Document::getStyle(const fm::String &name)
	{
		const Styles &styles = this->styles();
		// find style by name
		Styles::const_iterator it = _findByName(name, styles);
		if (it == styles.end()) {
			return StyleType();
		}
		return it->second;
	}

	IStyleDefServer::ConstChordValueType Document::getChord(const fm::String &name)
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

	IStyleDefServer::ConstPitchDefValueType Document::getAlias(fm::String alias)
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