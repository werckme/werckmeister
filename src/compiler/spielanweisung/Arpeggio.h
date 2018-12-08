#ifndef SPIELANWEISUNG_ARPEGGIO_HPP
#define SPIELANWEISUNG_ARPEGGIO_HPP

#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Arpeggio : public ASpielanweisung {
        public:
            Arpeggio() = default;
            virtual ~Arpeggio() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying = false) override;
            virtual void setArguments(const Event::Args &args) override;
            enum Direction { Up, Down };
            Direction direction = Up;
        };
    }
}

#endif