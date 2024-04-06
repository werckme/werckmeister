#include "BeginTag.h"
#include <com/common.hpp>

namespace compiler
{
    void BeginTag::execute(IContextPtr context)
    {
        auto tags = parameters[argumentNames.BeginTag.Tags].value<com::String>();
        com::StringStream ss(tags);
        com::String tag;
        while(ss >> tag) {
            context->addTag(tag);
        }
    }

}