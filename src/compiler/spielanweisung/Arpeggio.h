#ifndef SPIELANWEISUNG_ARPEGGIO_HPP
#define SPIELANWEISUNG_ARPEGGIO_HPP

#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Arpeggio : public ASpielanweisung {
        public:
            Arpeggio() = default;
            virtual ~Arpeggio() = default;
            virtual void perform(AContext *ctx, const Event &ev) override;
            virtual void setArguments(const Event::Args &args) override;
            enum Direction { Up, Down };
            Direction direction = Up;
        };
    }
}

#endif