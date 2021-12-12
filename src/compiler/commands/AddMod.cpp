#include "AddMod.h"
#include <compiler/context/IContext.h>
#include <compiler/modification/AModification.h>
#include <com/werckmeister.hpp>
#include <algorithm>

namespace compiler
{
    void AddMod::execute(IContextPtr context)
    {
        auto meta = context->voiceMetaData();
        meta->modifications.push_back(theModification);
    }

    void AddMod::setArguments(const Arguments &args)
    {
        Base::setArguments(args);
        com::String modName = parameters[argumentNames.AddMod.Use].value<com::String>();
        auto &wm = com::getWerckmeister();
        theModification = wm.getModification(modName);
        // remove "use" Argument from collection
        Arguments copy(args.size());
        bool paramUsefound = false;
        auto copiedIt = std::copy_if(args.begin(), args.end(), copy.begin(), [&paramUsefound](const auto &x)
                                     {
                                         if (!paramUsefound && x.name == argumentNames.AddMod.Use)
                                         {
                                             paramUsefound = true;
                                         }
                                         return x.name != argumentNames.AddMod.Use;
                                     });
        if (paramUsefound)
        {
            copy.resize(std::distance(copy.begin(), copiedIt));
        }
        else
        {
            // not found by name -> skip first arg
            copy = Arguments(copy.begin() + 1, copy.end());
        }
        theModification->setArguments(copy);
    }
}