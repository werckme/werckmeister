#pragma once

#include <com/IRegisterable.h>
#include <com/midi.hpp>
#include <documentModel/objects/ConductionSheetDef.h>
#include <memory>
#include <functional>
#include <compiler/error.hpp>

namespace conductor
{
    class IDeclaration : public com::IRegisterable
    {
    public:
        struct Events
        {
            com::midi::Event *noteOn = nullptr;
            com::midi::Event *noteOff = nullptr;
            com::midi::Event *predecessorNoteOn = nullptr;
            com::midi::Event *predecessorNoteOff = nullptr;
        };
        virtual void setDeclarationData(const ConductionRule::Declaration &) = 0;
        virtual const ConductionRule::Declaration &getDeclarationData() const = 0;
        virtual void perform(const Events &events) const = 0;
        // priority of a declaration higher values means higher priority
        virtual double specificity() const = 0;
        virtual void specificity(double val) = 0;
        virtual ~IDeclaration() = default;
    };
    typedef std::shared_ptr<IDeclaration> IDeclarationPtr;
}
