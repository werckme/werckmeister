#include "VoicingStrategy.h"
#include <algorithm>

namespace compiler
{
    bool containsVoicingStrategy(const VoicingStrategies& strategies, com::String strategyName)
    {
        return strategies.end() != std::find_if(strategies.begin(), strategies.end(), [strategyName](auto x)
        {
            return x->name() == strategyName;
        });
        
    }
}