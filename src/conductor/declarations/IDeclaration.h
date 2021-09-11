#ifndef IDECLARATION_H
#define IDECLARATION_H

#include <fm/IRegisterable.h>
#include <fm/midi.hpp>
#include <sheet/objects/ConductionSheetDef.h>
#include <memory>

namespace sheet
{
    namespace conductor
    {
        class IDeclaration : public fm::IRegisterable
        {
        public:
            ConductionRule::Declaration declaration;
            virtual void perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const = 0;
            virtual ~IDeclaration() = default;
        };
        typedef std::shared_ptr<IDeclaration> IDeclarationPtr;
    }
}

#endif