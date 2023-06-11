#include "WithTag.h"
#include <compiler/error.hpp>
#include <algorithm>

namespace conductor
{
    bool WithTag::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.midiEvent;
        auto tags = _eventInformationServer->getTags(ev);
        if (tags.empty())
        {
            return false;
        }        
		for (const auto &argument : arguments)
		{
            if (std::find(tags.begin(), tags.end(), argument.name) != tags.end()) 
            {
                return true;
            }
		}
		return false;
    }
}