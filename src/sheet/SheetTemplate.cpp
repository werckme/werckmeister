#include "SheetTemplate.h"
#include <algorithm>

namespace sheet {
    com::Ticks SheetTemplate::maxLength() const
    {
        com::Ticks max = 0;
        for (const auto &track : tracks) {
            max = std::max(max, track->maxLength());
		}
		return max;
    }
}