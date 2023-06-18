#include "TimeOffset.h"
#include <compiler/error.hpp>
#include <limits>

namespace conductor
{
    void TimeOffset::perform(const Events &events) const
    {
        com::midi::Event *midiEvent = events.midiEvent;
        if (midiEvent->eventType() != com::midi::NoteOn)
        {
            midiEvent->absPosition(std::max(com::Ticks(0), (midiEvent->absPosition() + declaration.value * com::PPQ))); 
            return;
        }
        com::midi::Event *noteOff = events.noteOff;
        const com::midi::Event &originalNoteOn = events.unmodifiedOriginalMidiEvent;
        const com::midi::Event &originalNoteOff = events.unmodifiedOriginalNoteOff;
        if (!noteOff)
        {
            return;
        }
        FGetValue getOriginalValue = []() { return 0; };
        FGetValue getPercentBase = [originalNoteOn, originalNoteOff]()
        { return (originalNoteOff.absPosition() - originalNoteOn.absPosition()) / com::PPQ; };
        FSetValue setNoteOn = [](com::midi::Event *midiEvent, double val)
        { midiEvent->absPosition(std::max(com::Ticks(0), (midiEvent->absPosition() + val * com::PPQ))); };
        FSetValue setNoteOff = [midiEvent](com::midi::Event *noteOff, double val)
        { noteOff->absPosition(std::max(com::Ticks(0), (noteOff->absPosition() + val * com::PPQ))); };
        FGetOptionalValue getPredecessorValue = []() -> std::optional<double> {
            FM_THROW(compiler::Exception, "the follow up operator isn't supported by timeOffset");
        };
        double inputValue = declaration.value;
        constexpr double min = -std::numeric_limits<double>::max();
        constexpr double max = std::numeric_limits<double>::max();
        performImpl(midiEvent, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
    }

    bool TimeOffset::canApply(const com::midi::Event& event)
    {
        return !com::midi::isDeviceConfigEvent(event);
    }
}