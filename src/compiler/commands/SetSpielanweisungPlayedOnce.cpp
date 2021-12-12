#include "SetSpielanweisungPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void SetSpielanweisungPlayedOnce::execute(IContextPtr context)
    {
        auto meta = context->voiceMetaData();
        meta->spielanweisungOnce = theModification;
    }
}