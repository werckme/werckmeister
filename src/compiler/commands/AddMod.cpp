#include "AddMod.h"
#include <compiler/context/AContext.h>
#include <compiler/modification/AModification.h>
#include <fm/werckmeister.hpp>
#include <algorithm>

namespace sheet {
    namespace compiler {
        void AddMod::execute(AContext* context)
        {
			auto meta = context->voiceMetaData();
			meta->modifications.push_back(theModification);
        }

        void AddMod::setArguments(const Arguments &args)
        {
            Base::setArguments(args);
            fm::String modName = parameters[argumentNames.AddMod.Use].value<fm::String>();
            auto &wm = fm::getWerckmeister();
            theModification = wm.getModification(modName);
            // remove "use" Argument from collection
            Arguments copy (args.size());
            bool paramUsefound = false;
            auto copiedIt = std::copy_if(args.begin(), args.end(), copy.begin(), [&paramUsefound](const auto &x) {
                if (!paramUsefound && x.name == argumentNames.AddMod.Use) {
                    paramUsefound = true;
                }
                return x.name != argumentNames.AddMod.Use;
            });
            if (paramUsefound) {
                copy.resize(std::distance(copy.begin(), copiedIt));
            } else {
                // not found by name -> skip first arg
                copy = Arguments(copy.begin() + 1, copy.end());
            }
            theModification->setArguments(copy);
        }
    }
}