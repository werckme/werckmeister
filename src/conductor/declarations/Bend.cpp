#include "Bend.h"
#include <com/midi.hpp>
#include <cassert>
namespace conductor
{
    Bend::Bend(com::midi::MidiPtr midi) : midi(midi) 
    {
    }

    void Bend::perform(const Events &events) const
    {
        
        if (declaration.unit == documentModel::ConductionRule::Declaration::UnitPercent)
        {
            FM_THROW(compiler::Exception, "the unit precent isn't supported by bend");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationAdd)
        {
            FM_THROW(compiler::Exception, "the add operator isn't supported by bend");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationSubstract)
        {
            FM_THROW(compiler::Exception, "the substract operator isn't supported by bend");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpAdd)
        {
            FM_THROW(compiler::Exception, "the follow up add operator isn't supported by bend");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract)
        {
            FM_THROW(compiler::Exception, "the follow up substract operator isn't supported by bend");
        }
        auto noteOn = events.midiEvent;
        auto eventPosition = noteOn->absPosition();
        auto track = noteOn->parentTrack;
        assert(track != nullptr);
        auto val = declaration.numberValue() / 100.0;
        if (val < 0 || val > 1.0) 
        {
            FM_THROW(compiler::Exception, "invalid bend value (0..100): " + std::to_string(declaration.numberValue()));
        }
        auto bendEvent = com::midi::Event::PitchBend(noteOn->channel(), eventPosition, val);
        track->events().add(bendEvent);
    }
}