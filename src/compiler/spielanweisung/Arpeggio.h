#ifndef SPIELANWEISUNG_ARPEGGIO_HPP
#define SPIELANWEISUNG_ARPEGGIO_HPP

#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Arpeggio : public ASpielanweisung {
        public:
            Arpeggio() = default;
            virtual ~Arpeggio() = default;
            virtual void perform(IContextPtr ctx, Events &events) override;
            enum Direction { Up, Down };
            Direction direction = Up;
            com::IHasParameter::ParametersByNames parameters = {
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }            
        };
    }
}

#endif