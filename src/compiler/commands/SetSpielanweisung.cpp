#include "SetSpielanweisung.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetSpielanweisung::execute(AContext* context)
        {
            auto meta = context->voiceMetaData();
			meta->spielanweisung = theModification;
        }
    }
}