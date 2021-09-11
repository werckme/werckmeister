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
            ConductionRule::Declaration declaration;
            virtual void perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const = 0;
            virtual ~IDeclaration() = default;
        protected:
            typedef std::function<double(double)> FNormalize;
            typedef std::function<double(fm::midi::Event*)> FGetValue;
            typedef std::function<void(fm::midi::Event*, double)> FSetValue;
            inline void performImpl(fm::midi::Event* noteOn, fm::midi::Event* noteOff, const FNormalize &normalizeValue, const FGetValue &getOriginalValue, const FGetValue &getPercentBaseValue, const FSetValue &setNoteOnValue, const FSetValue &setNoteOffValue) const
            {
                double value = normalizeValue(declaration.value);
                if (declaration.unit == ConductionRule::Declaration::UnitPercent) {
                    value = value * getOriginalValue(noteOn) / 100;
                }
                if (declaration.operation == ConductionRule::Declaration::OperationAdd) {
                    auto originalValue = getOriginalValue(noteOn);
                    value = originalValue + value;
                }
                if (declaration.operation == ConductionRule::Declaration::OperationSubstract) {
                    auto originalValue = getOriginalValue(noteOn);
                    value = originalValue - value;
                }
                value = std::max(0.0, std::min(value, 1.0));
                setNoteOnValue(noteOn, value);
                if (noteOff) {
                    setNoteOffValue(noteOff, value);
                }
            }
        };
        typedef std::shared_ptr<IDeclaration> IDeclarationPtr;
    }
}

#endif