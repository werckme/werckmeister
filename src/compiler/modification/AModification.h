#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <fm/units.hpp>
#include <sheet/objects/Event.h>
#include <list>
#include <fm/IRegisterable.h>
#include <fm/AConvertsArgumentsToParameter.h>

namespace sheet {
    namespace compiler {
        class AContext;
        class AModification : public fm::IRegisterable, public fm::AConvertsArgumentsToParameter {
        protected:
            AModification() = default;
        public:
            typedef std::list<Event> Events;
            virtual ~AModification() = default;
            virtual void perform(AContext *ctx, Events &events) = 0;
        };
    }
}

#endif