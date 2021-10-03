#include "Duration.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Duration::perform(const Events &events) const
        {
            fm::midi::Event* noteOn = events.noteOn;
            fm::midi::Event* noteOff = events.noteOff;
            fm::midi::Event* predecessorNoteOn = events.predecessorNoteOn;
            fm::midi::Event* predecessorNoteOff = events.predecessorNoteOff;
            if (!noteOff) 
            {
                return;
            }
            FGetValue getOriginalValue = [noteOn, noteOff]() { return (noteOff->absPosition() - noteOn->absPosition()) / fm::PPQ; };
            FGetValue getPercentBase = [noteOn, noteOff]() { return (noteOff->absPosition() - noteOn->absPosition()) / fm::PPQ; };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) {};
            FSetValue setNoteOff = [noteOn](fm::midi::Event* noteOff, double val) { noteOff->absPosition(noteOn->absPosition() + val * fm::PPQ); };
            FGetOptionalValue getPredecessorValue = [predecessorNoteOn, predecessorNoteOff]() -> std::optional<double>
            {
                if (!predecessorNoteOn || !predecessorNoteOff) 
                {
                    return std::nullopt;
                }
                return (predecessorNoteOff->absPosition() - predecessorNoteOn->absPosition()) / fm::PPQ; 
            };
            double inputValue = declaration.value;
            constexpr double min = 0.000000001;
            constexpr double max = std::numeric_limits<double>::max();
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
        }
    }
}