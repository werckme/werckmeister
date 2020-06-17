#ifndef FMAPP_SHEETWATCHER_HPP
#define FMAPP_SHEETWATCHER_HPP


#include "IPlayerLoopVisitor.h"
#include <unordered_map>
#include <fm/common.hpp>
#include <time.h>
#include <forward.hpp>


namespace fmapp {
    /**
     * watches a sheetfile and notify if any changes where done
     */
    class SheetWatcher : public IPlayerLoopVisitor {
    private:
        sheet::DocumentPtr _document;
    public:
        SheetWatcher(sheet::DocumentPtr document) 
        : _document(document) 
        {}
        virtual void visit(fm::Ticks elapsed);
        virtual ~SheetWatcher() = default;
    private:
        typedef std::unordered_map<fm::String, time_t> Timestamps;
        Timestamps timestamps;
        void updateLastChangedTimestamp();
        time_t getTimestamp(const fm::String &sheetPath);
        bool hasChanges();
        unsigned long _lastUpdateTimestamp = 0;
    };
}
#endif