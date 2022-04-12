#include "TimeOffset.h"
#include <compiler/error.hpp>
#include <limits>

namespace conductor
{
    void TimeOffset::perform(const Events &events) const
    {
        com::midi::Event *noteOn = events.noteOn;
        com::midi::Event *noteOff = events.noteOff;
        const com::midi::Event &originalNoteOn = events.unmodifiedOriginalNoteOn;
        const com::midi::Event &originalNoteOff = events.unmodifiedOriginalNoteOff;
        if (!noteOff)
        {
            return;
        }
        FGetValue getOriginalValue = [noteOn]()
        { return 0; };
        FGetValue getPercentBase = [originalNoteOn, originalNoteOff]()
        { return (originalNoteOff.absPosition() - originalNoteOn.absPosition()) / com::PPQ; };
        FSetValue setNoteOn = [](com::midi::Event *noteOn, double val)
        { noteOn->absPosition(std::max(com::Ticks(0), (noteOn->absPosition() + val * com::PPQ))); };
        FSetValue setNoteOff = [noteOn](com::midi::Event *noteOff, double val)
        { noteOff->absPosition(std::max(com::Ticks(0), (noteOff->absPosition() + val * com::PPQ))); };
        FGetOptionalValue getPredecessorValue = []() -> std::optional<double> {
            FM_THROW(compiler::Exception, "the follow up operator isn't supported by timeOffset");
        };
        double inputValue = declaration.value;
        constexpr double min = -std::numeric_limits<double>::max();
        constexpr double max = std::numeric_limits<double>::max();
        performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
    }
}