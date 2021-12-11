#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <com/units.hpp>
#include <sheet/objects/Event.h>
#include <list>
#include <com/IRegisterable.h>
#include <com/AConvertsArgumentsToParameter.h>
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        class AModification : public com::IRegisterable, public com::AConvertsArgumentsToParameter {
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