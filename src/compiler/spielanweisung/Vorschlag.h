#ifndef SPIELANWEISUNG_VORSCHLAG_HPP
#define SPIELANWEISUNG_VORSCHLAG_HPP

#include <sheet/objects/Event.h>
#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Vorschlag : public ASpielanweisung {
        public:
            typedef AModification Base;
            static const fm::Ticks defaultDuration;
            Vorschlag() = default;
            virtual ~Vorschlag() = default;
            virtual void perform(AContext *ctx, Events &events) override;
            Event vorschlagNote;
        };
    }
}

#endif