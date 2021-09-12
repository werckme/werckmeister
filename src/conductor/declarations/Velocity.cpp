#include "Velocity.h"
#include <compiler/error.hpp>
#include <fm/midi.hpp>

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const
        {
            FGetValue getOriginalValue = [noteOn]() { return noteOn->parameter2(); };
            FGetValue getPercentBase = [noteOn]() { return noteOn->parameter2(); };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) { noteOn->parameter2(fm::Byte(val)); };
            FSetValue setNoteOff = [](fm::midi::Event* noteOn, double val) { };
            double inputValue = declaration.value;
            double min = 0.0;
            double max = double(fm::midi::MaxMidiValue);
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, setNoteOn, setNoteOff);
        }
    }
}