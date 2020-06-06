#include "SetSpielanweisungPlayedOnce.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void SetSpielanweisungPlayedOnce::execute(AContext* context)
        {
            auto meta = context->voiceMetaData();
			meta->spielanweisungOnce = theModification;
        }
    }
}