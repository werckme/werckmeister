#include "Duration.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Duration::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const
        {
            FGetValue getOriginalValue = [noteOn]() { return 0; };
            FGetValue getPercentBase = [noteOn, noteOff]() { return (noteOff->absPosition() - noteOn->absPosition()) / fm::PPQ; };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) {};
            FSetValue setNoteOff = [noteOn](fm::midi::Event* noteOff, double val) { noteOff->absPosition(noteOff->absPosition() + val * fm::PPQ); };
            double inputValue = declaration.value;
            constexpr double min = -std::numeric_limits<double>::max();
            constexpr double max = std::numeric_limits<double>::max();
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, setNoteOn, setNoteOff);
        }
    }
}