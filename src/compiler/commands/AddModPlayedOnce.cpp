#include "AddModPlayedOnce.h"
#include <compiler/context/IContext.h>


namespace sheet {
    namespace compiler {
        void AddModPlayedOnce::execute(IContextPtr  context)
        {
			auto meta = context->voiceMetaData();
			meta->modificationsOnce.push_back(theModification);
        }
    }
}