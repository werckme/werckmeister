#include "ADeclaration.h"

namespace conductor
{
    double ADeclaration::specificity() const
    {
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpAdd ||
            declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract)
        {
            return 0.0;
        }
        return _specificity;
    }
    void ADeclaration::performImpl(com::midi::Event *noteOn,
                                   com::midi::Event *noteOff,
                                   double inputValue,
                                   double min, double max,
                                   const FGetValue &getOriginalValue,
                                   const FGetValue &getPercentBaseValue,
                                   const FGetOptionalValue &getPredecessorValue,
                                   const FSetValue &setNoteOnValue,
                                   const FSetValue &setNoteOffValue) const
    {
        if (declaration.unit == documentModel::ConductionRule::Declaration::UnitPercent)
        {
            auto percentBaseValue = getPercentBaseValue();
            inputValue = inputValue * percentBaseValue / 100;
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationAdd)
        {
            auto originalValue = getOriginalValue();
            inputValue = originalValue + inputValue;
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationSubstract)
        {
            auto originalValue = getOriginalValue();
            inputValue = originalValue - inputValue;
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpAdd || declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract)
        {
            auto predecessorValue = getPredecessorValue();
            if (!predecessorValue.has_value())
            {
                return;
            }
            inputValue = declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract
                             ? -inputValue
                             : inputValue;
            inputValue = predecessorValue.value() + inputValue;
        }
        inputValue = std::max(min, std::min(inputValue, max));
        setNoteOnValue(noteOn, inputValue);
        if (noteOff)
        {
            setNoteOffValue(noteOff, inputValue);
        }
    }
    
    bool ADeclaration::canApply(const com::midi::Event& event)
    {
        return event.eventType() == com::midi::NoteOn;
    }
}
