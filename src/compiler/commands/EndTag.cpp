#include "EndTag.h"
#include <com/common.hpp>

namespace compiler
{
    void EndTag::execute(IContextPtr context)
    {
        auto tags = parameters[argumentNames.BeginTag.Tags].value<com::String>();
        com::StringStream ss(tags);
        com::String tag;
        while(ss >> tag) {
            context->removeTag(tag);
        }
    }

}