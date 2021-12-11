#ifndef FMAPP_SHEETWATCHER_HPP
#define FMAPP_SHEETWATCHER_HPP


#include "IPlayerLoopVisitor.h"
#include <unordered_map>
#include <com/common.hpp>
#include <time.h>
#include <forward.hpp>
#include "ISheetWatcherHandler.h"

namespace app {
    /**
     * watches a sheetfile and notify if any changes where done
     */
    class SheetWatcher : public IPlayerLoopVisitor {
    private:
        sheet::DocumentPtr               _document;
        app::SheetWatcherHandlersPtr   _sheetWatcherHandlers;
    public:
        SheetWatcher(sheet::DocumentPtr document, app::SheetWatcherHandlersPtr sheetWatcherHandlers) 
        : _document(document), _sheetWatcherHandlers(sheetWatcherHandlers)
        {}
        virtual void loopBegin() override {}
        virtual void loopEnd() override {}
        virtual void visit(com::Ticks elapsed);
        virtual ~SheetWatcher() = default;
    private:
        typedef std::unordered_map<com::String, time_t> Timestamps;
        Timestamps timestamps;
        void updateLastChangedTimestamp();
        time_t getTimestamp(const com::String &sheetPath);
        bool hasChanges();
        unsigned long _lastUpdateTimestamp = 0;
        bool firstVisit = true;
    };
}
#endif