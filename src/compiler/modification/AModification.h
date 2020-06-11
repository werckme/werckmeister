#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <fm/units.hpp>
#include <sheet/objects/Event.h>
#include <list>
#include <fm/IRegisterable.h>
#include <fm/AConvertsArgumentsToParameter.h>

namespace sheet {
    namespace compiler {
        class IContext;
        class AModification : public fm::IRegisterable, public fm::AConvertsArgumentsToParameter {
        protected:
            AModification() = default;
        public:
            typedef std::list<Event> Events;
            virtual ~AModification() = default;
            virtual void perform(IContext *ctx, Events &events) = 0;
        };
    }
}

#endif