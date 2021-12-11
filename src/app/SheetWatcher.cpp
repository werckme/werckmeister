#include "SheetWatcher.h"
#include <boost/filesystem.hpp>
#include <sheet/Document.h>
#include <com/werckmeister.hpp>
#include <iostream>

namespace app {

    void SheetWatcher::updateLastChangedTimestamp() 
    {
        _lastUpdateTimestamp = (unsigned long)time(NULL);
    }

    time_t SheetWatcher::getTimestamp(const com::String &sheetPath)
    {
        auto path = boost::filesystem::path(sheetPath);
        return boost::filesystem::last_write_time(path);
    }

    bool SheetWatcher::hasChanges()
    {
        auto changed = [&] (const com::String &path) {
            time_t new_timestamp = getTimestamp(path);
            auto it = timestamps.find(path);
            if (it == timestamps.end()) {
                timestamps.emplace(std::make_pair(path, new_timestamp));
                return true;
            }
            if (it->second != getTimestamp(path)) {
                timestamps[path] = new_timestamp;
                return true;
            }
            return false;
        };
        bool result = changed(_document->path);
        // check all files, to update their timestamps
        for(const auto &p : _document->sheetDef.documentUsing.usings) {
            auto fullPath = com::getWerckmeister().resolvePath(p);
            result |= changed(fullPath);
        }
        return result;
    }

    void SheetWatcher::visit(com::Ticks elapsed)
    {
        if (!this->hasChanges()) {
            return;
        }
        if (firstVisit) {
            // ignore first visit
            firstVisit = false;
            return;
        }
        for(auto sheetWatcher : _sheetWatcherHandlers->container) {
            sheetWatcher->onSheetChanged();
        }
    }

}