#include "SetSpielanweisung.h"
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        void SetSpielanweisung::execute(IContextPtr  context)
        {
            auto meta = context->voiceMetaData();
			meta->spielanweisung = theModification;
        }
    }
}