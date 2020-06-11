#include "SetSpielanweisungPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        void SetSpielanweisungPlayedOnce::execute(IContext* context)
        {
            auto meta = context->voiceMetaData();
			meta->spielanweisungOnce = theModification;
        }
    }
}