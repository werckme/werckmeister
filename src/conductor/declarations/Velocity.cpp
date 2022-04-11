#include "Velocity.h"
#include <compiler/error.hpp>
#include <com/midi.hpp>

namespace conductor
{
    void Velocity::perform(const Events &events) const
    {
        com::midi::Event *noteOn = events.noteOn;
        com::midi::Event *noteOff = events.noteOff;
        com::midi::Event *predecessorNoteOn = events.predecessorNoteOn;
        const com::midi::Event &originalNoteOn = events.unmodifiedOriginalNoteOn;
        const com::midi::Event &originalNoteOff = events.unmodifiedOriginalNoteOff;
        if (!noteOff)
        {
            return;
        }
        FGetValue getOriginalValue = [noteOn]()
        { return noteOn->parameter2(); };
        FGetValue getPercentBase = [originalNoteOn]()
        { return originalNoteOn.parameter2(); };
        FSetValue setNoteOn = [](com::midi::Event *noteOn, double val)
        { noteOn->parameter2(com::Byte(val)); };
        FSetValue setNoteOff = [](com::midi::Event *noteOff, double val) {};
        FGetOptionalValue getPredecessorValue = [predecessorNoteOn]() -> std::optional<double>
        {
            if (!predecessorNoteOn)
            {
                return std::nullopt;
            }
            return predecessorNoteOn->parameter2();
        };
        double inputValue = declaration.value;
        constexpr double min = 0.0;
        constexpr double max = double(com::midi::MaxMidiValue);
        performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
    }
}