#include "Pitch.h"
#include <compiler/error.hpp>
#include <fm/midi.hpp>

namespace sheet
{
    namespace conductor
    {
        void PitchDecl::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const
        {
             FGetValue getOriginalValue = [noteOn]() { return noteOn->parameter1(); };
             FGetValue getPercentBase = [noteOn]() { return noteOn->parameter1(); };
             FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) { noteOn->parameter1(fm::Byte(val)); };
             FSetValue setNoteOff = [](fm::midi::Event* noteOff, double val) { noteOff->parameter1(fm::Byte(val)); };
             double inputValue = declaration.value;
             constexpr double min = 0.0;
             constexpr double max = double(fm::midi::MaxMidiValue);
             performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, setNoteOn, setNoteOff);
        }
    }
}