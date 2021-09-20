#include "Velocity.h"
#include <compiler/error.hpp>
#include <fm/midi.hpp>

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(const Events &events) const
        {
            fm::midi::Event* noteOn = events.noteOn;
            fm::midi::Event* noteOff = events.noteOff;
            fm::midi::Event* predecessorNoteOn = events.predecessorNoteOn;
            if (!noteOff) 
            {
                return;
            }
            FGetValue getOriginalValue = [noteOn]() { return noteOn->parameter2(); };
            FGetValue getPercentBase = [noteOn]() { return noteOn->parameter2(); };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) { noteOn->parameter2(fm::Byte(val)); };
            FSetValue setNoteOff = [](fm::midi::Event* noteOff, double val) { };
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
            constexpr double max = double(fm::midi::MaxMidiValue);
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
        }
    }
}