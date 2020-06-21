#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <fm/units.hpp>
#include <sheet/objects/Event.h>
#include <list>
#include <fm/IRegisterable.h>
#include <fm/AConvertsArgumentsToParameter.h>
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        class AModification : public fm::IRegisterable, public fm::AConvertsArgumentsToParameter {
        protected:
            AModification() = default;
        public:
            typedef std::list<Event> Events;
            virtual ~AModification() = default;
            virtual void perform(IContextPtr ctx, Events &events) = 0;
        };
    }
}

#endif