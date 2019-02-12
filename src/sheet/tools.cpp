#include "tools.h"
#include "Event.h"

namespace sheet {

    namespace toolsimpl {

        const std::vector<fm::String> & getMetaArgs(const Event &metaEvent)
        {
            return metaEvent.metaArgs;
        }
        const fm::String & getMetaCommand(const Event &metaEvent)
        {
            return metaEvent.stringValue;
        }
    }

}