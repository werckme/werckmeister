#include "Velocity.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const
        {
            FNormalize normalizeInValue = [](double inputValue) { return inputValue / 100.; };
            FGetValue getOriginalValue = [](fm::midi::Event* noteOn) { return noteOn->parameter2() / double(fm::midi::MaxMidiValue); };
            FGetValue getPercentBase = [](fm::midi::Event* noteOn) { return noteOn->parameter2() / double(fm::midi::MaxMidiValue); };
            FSetValue setNoteOn = [](fm::midi::Event* noteOn, double val) { noteOn->parameter2( fm::Byte(val * fm::midi::MaxMidiValue) ); };
            FSetValue setNoteOff = [](fm::midi::Event* noteOn, double val) { };
            performImpl(noteOn, noteOff, normalizeInValue, getOriginalValue, getPercentBase, setNoteOn, setNoteOff);

        }
    }
}