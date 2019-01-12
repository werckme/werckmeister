#ifndef SPIELANWEISUNG_VORSCHLAG_HPP
#define SPIELANWEISUNG_VORSCHLAG_HPP

#include "Normal.h"
#include <sheet/Event.h>

namespace sheet {
    namespace compiler {
        class Vorschlag : public Normal {
        public:
            static const fm::Ticks defaultDuration;
            typedef Normal Base;
            Vorschlag() = default;
            virtual ~Vorschlag() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying = false) override;
            Event vorschlagNote;
        };
    }
}

#endif