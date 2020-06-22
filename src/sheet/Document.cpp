#include "Document.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fm/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>
#include <functional>
#include <fm/werckmeister.hpp>
#include <compiler/Preprocessor.h>
#include <compiler/error.hpp>

namespace sheet {


	fm::String Document::getAbsolutePath(const fm::String &path) const
	{
		sheet::ConstDocumentPtr thisPtr = shared_from_this();
		return fm::getWerckmeister().resolvePath(path, thisPtr);
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