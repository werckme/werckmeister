#include "SetVoicingStrategy.h"
#include <compiler/context/IContext.h>
#include <com/werckmeister.hpp>
#include <algorithm>

namespace compiler
{
    void SetVoicingStrategy::execute(IContextPtr context)
    {
        auto meta = context->voiceMetaData();
        bool isAlreadySet = containsVoicingStrategy(meta->voicingStrategies, voicingStrategy);
        if (isAlreadySet)
        {
            return;
        }
        meta->voicingStrategies.push_back(this->voicingStrategy);
    }

    void SetVoicingStrategy::setArguments(const Arguments &args)
    {
        Base::setArguments(args);
        auto voicingStrategyName = parameters[argumentNames.SetVoicingStrategy.Use].value<com::String>();
        this->voicingStrategy = com::getWerckmeister().getVoicingStrategy(voicingStrategyName);
        if (args.size() <= 1)
        {
            return;
        }
        Arguments remaining(args.begin() + 1, args.end());
        this->voicingStrategy->setArguments(remaining);
    }
}