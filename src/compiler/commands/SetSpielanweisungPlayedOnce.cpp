#include "SetSpielanweisungPlayedOnce.h"
#include <compiler/context/IContext.h>

namespace compiler
{
    void SetSpielanweisungPlayedOnce::execute(IContextPtr context)
    {
        com::String modName = parameters[argumentNames.AddMod.Use].value<com::String>();
        auto meta = context->voiceMetaData();
        auto theModification = loadMod(context, modName);
        meta->spielanweisungOnce = theModification;
    }
}