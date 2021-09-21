#ifndef IDECLARATION_H
#define IDECLARATION_H

#include <fm/IRegisterable.h>
#include <fm/midi.hpp>
#include <sheet/objects/ConductionSheetDef.h>
#include <memory>
#include <functional>
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        class IDeclaration : public fm::IRegisterable
        {
        public:
            struct Events {
                fm::midi::Event* noteOn = nullptr;
                fm::midi::Event* noteOff = nullptr;
                fm::midi::Event* predecessorNoteOn = nullptr;
                fm::midi::Event* predecessorNoteOff = nullptr;
            };
            virtual void setDeclarationData(const ConductionRule::Declaration&) = 0;
            virtual const ConductionRule::Declaration& getDeclarationData() const = 0;
            virtual void perform(const Events &events) const = 0;
            // priority of a declaration higher values means higher priority
            virtual int priority() const = 0;
            virtual ~IDeclaration() = default;
        };
        typedef std::shared_ptr<IDeclaration> IDeclarationPtr;
    }
}

#endif