#include "WithTag.h"
#include <compiler/error.hpp>
#include <algorithm>

namespace conductor
{
    bool WithTag::isMatch(const documentModel::ConductionSelector::Arguments &arguments, const EventWithMetaInfo &evm) const
    {
        const auto &ev = *evm.noteOn;
		for (const auto &argument : arguments)
		{
			auto tags = _eventInformationServer->getTags(ev);
            if (tags.empty())
            {
                continue;
            }
            if (std::find(tags.begin(), tags.end(), argument.name) != tags.end()) 
            {
                return true;
            }
		}
		return false;
    }
}