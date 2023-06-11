#include "Duration.h"
#include <compiler/error.hpp>

namespace conductor
{
    void Duration::perform(const Events &events) const
    {
        com::midi::Event *noteOn = events.midiEvent;
        com::midi::Event *noteOff = events.noteOff;
        com::midi::Event *predecessorNoteOn = events.predecessorNoteOn;
        com::midi::Event *predecessorNoteOff = events.predecessorNoteOff;
        const com::midi::Event &originalNoteOn = events.unmodifiedOriginalMidiEvent;
        const com::midi::Event &originalNoteOff = events.unmodifiedOriginalNoteOff;
        if (!noteOff)
        {
            return;
        }
        FGetValue getOriginalValue = [originalNoteOn, originalNoteOff]()
        { return (originalNoteOff.absPosition() - originalNoteOn.absPosition()) / com::PPQ; };
        FGetValue getPercentBase = [originalNoteOn, originalNoteOff]()
        { return (originalNoteOff.absPosition() - originalNoteOn.absPosition()) / com::PPQ; };
        FSetValue setNoteOn = [](com::midi::Event *noteOn, double val) {};
        FSetValue setNoteOff = [noteOn](com::midi::Event *noteOff, double val)
        { noteOff->absPosition(noteOn->absPosition() + val * com::PPQ); };
        FGetOptionalValue getPredecessorValue = [predecessorNoteOn, predecessorNoteOff]() -> std::optional<double>
        {
            if (!predecessorNoteOn || !predecessorNoteOff)
            {
                return std::nullopt;
            }
            return (predecessorNoteOff->absPosition() - predecessorNoteOn->absPosition()) / com::PPQ;
        };
        double inputValue = declaration.value;
        constexpr double min = 0.000000001;
        constexpr double max = std::numeric_limits<double>::max();
        performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
    }
}