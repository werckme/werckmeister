#ifndef FMAPP_SHEETWATCHER_HPP
#define FMAPP_SHEETWATCHER_HPP


#include "IPlayerLoopVisitor.h"

namespace fmapp {
    /**
     * watches a sheetfile and notify if any changes where done
     */
    class SheetWatcher : public IPlayerLoopVisitor {
    public:
        virtual void visit(fm::Ticks elapsed);
        virtual ~SheetWatcher() = default;
    };
}
#endif