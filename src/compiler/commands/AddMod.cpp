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
            fm::String modName = parameters[argumentNames.AddModPlayedOnce.Use].value<fm::String>();
            auto &wm = fm::getWerckmeister();
            theModification = wm.getModification(modName);
            if (args.size() <= 1) {
                return;
            }

            // remove "use" Argument from collection
            Arguments copy (args.size());
            auto copiedIt = std::copy_if(args.begin(), args.end(), copy.begin(), [](const auto &x) {  
                return x.name != argumentNames.AddModPlayedOnce.Use;
            });
            copy.resize(std::distance(copy.begin(), copiedIt));
            theModification->setArguments(copy);
        }
    }
}