#pragma once

#include "DiContainerWrapper.h"
#include <memory>

namespace app
{
    /**
     * watches a sheetfile and notify if any changes where done
     */
    class ISheetWatcherHandler
    {
    public:
        virtual void onSheetChanged() = 0;
    };
    typedef DiContainerWrapper<ISheetWatcherHandler *> SheetWatcherHandlers;
    typedef std::shared_ptr<SheetWatcherHandlers> SheetWatcherHandlersPtr;
}
