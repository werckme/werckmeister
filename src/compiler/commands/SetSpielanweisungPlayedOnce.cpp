#include "SetSpielanweisungPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void SetSpielanweisungPlayedOnce::execute(IContextPtr  context)
        {
            auto meta = context->voiceMetaData();
			meta->spielanweisungOnce = theModification;
        }
    }
}