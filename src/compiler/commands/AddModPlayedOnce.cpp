#include "AddModPlayedOnce.h"
#include <compiler/context/AContext.h>


namespace sheet {
    namespace compiler {
        void AddModPlayedOnce::execute(AContext* context)
        {
			auto meta = context->voiceMetaData();
			meta->modificationsOnce.push_back(theModification);
        }
    }
}