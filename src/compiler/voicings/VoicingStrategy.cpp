#include "VoicingStrategy.h"
#include <algorithm>

namespace compiler
{
    bool containsVoicingStrategy(const VoicingStrategies& strategies, VoicingStrategyPtr strategy)
    {
        return strategies.end() != std::find_if(strategies.begin(), strategies.end(), [strategy](auto x)
        {
            bool nameMatch  = x->name() == strategy->name();
            if (!nameMatch) 
            {
                return false;
            }
            return true;
        });
        
    }
}