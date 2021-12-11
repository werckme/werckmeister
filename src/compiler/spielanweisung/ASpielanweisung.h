#pragma once

#include <com/units.hpp>
#include <documentModel/objects/Event.h>
#include <compiler/modification/AModification.h>

namespace documentModel {
    namespace compiler {
        class AContext;
        class ASpielanweisung : public AModification {
        protected:
            ASpielanweisung() = default;
        public:
            virtual ~ASpielanweisung() = default;
        };
    }
}

