#include "TimeOffset.h"
#include <compiler/error.hpp>
#include <limits>

namespace sheet
{
    namespace conductor
    {
        void TimeOffset::perform(const Events &events) const
        {
            fm::midi::Event* noteOn = events.noteOn;
            fm::midi::Event* noteOff = events.noteOff;
            if (!noteOff) 
            {
                return;
            }
            FGetValue getOriginalValue = [noteOn]() { return 0; };
            FGetValue getPercentBase = [noteOn, noteOff]() { return (noteOff->absPosition() - noteOn->absPosition()) / fm::PPQ; };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) { noteOn->absPosition(std::max(fm::Ticks(0), (noteOn->absPosition() + val * fm::PPQ))); };
            FSetValue setNoteOff = [noteOn](fm::midi::Event* noteOff, double val) { noteOff->absPosition(std::max(fm::Ticks(0), (noteOff->absPosition() + val * fm::PPQ))); };
            FGetOptionalValue getPredecessorValue = []() -> std::optional<double>
            {
                FM_THROW(compiler::Exception, "the follow up operator isn't supported by timeOffset");
            };
            double inputValue = declaration.value;
            constexpr double min = -std::numeric_limits<double>::max();
            constexpr double max =  std::numeric_limits<double>::max();
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
        }
    }
}