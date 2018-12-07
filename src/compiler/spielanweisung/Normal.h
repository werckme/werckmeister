#ifndef SPIELANWEISUNG_NORMAL_HPP
#define SPIELANWEISUNG_NORMAL_HPP

#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Normal : public ASpielanweisung {
        public:
            Normal() = default;
            virtual ~Normal() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying = false) override;
        };
    }
}

#endif