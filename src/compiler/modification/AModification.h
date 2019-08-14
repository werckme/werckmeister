#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <fm/units.hpp>
#include "sheet/Event.h"

namespace sheet {
    namespace compiler {
        class AContext;
        class AModification {
        protected:
            AModification() = default;
        public:
            virtual ~AModification() = default;
            virtual void perform(AContext *ctx, const Event &ev) = 0;
            virtual void setArguments(const Event::Args &args) {}
        };
    }
}

#endif