#include "SheetTemplate.h"
#include <algorithm>

namespace sheet {
    fm::Ticks SheetTemplate::maxLength() const
    {
        fm::Ticks max = 0;
        for (const auto &track : tracks) {
            max = std::max(max, track->maxLength());
		}
		return max;
    }
}