#pragma once

#include <documentModel/objects/Event.h>
#include "ASpielanweisung.h"

namespace compiler
{
    class Vorschlag : public ASpielanweisung
    {
    public:
        typedef AModification Base;
        static const com::Ticks defaultDuration;
        Vorschlag() = default;
        virtual ~Vorschlag() = default;
        virtual void perform(IContextPtr ctx, Events &events) override;
        documentModel::Event vorschlagNote;
        com::IHasParameter::ParametersByNames parameters = {};
        virtual ParametersByNames &getParameters() { return this->parameters; }
    };
}
