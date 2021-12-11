#ifndef SPIELANWEISUNG_VORSCHLAG_HPP
#define SPIELANWEISUNG_VORSCHLAG_HPP

#include <documentModel/objects/Event.h>
#include "ASpielanweisung.h"

namespace documentModel {
    namespace compiler {
        class Vorschlag : public ASpielanweisung {
        public:
            typedef AModification Base;
            static const com::Ticks defaultDuration;
            Vorschlag() = default;
            virtual ~Vorschlag() = default;
            virtual void perform(IContextPtr ctx, Events &events) override;
            Event vorschlagNote;
            com::IHasParameter::ParametersByNames parameters = {
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }            
        };
    }
}

#endif