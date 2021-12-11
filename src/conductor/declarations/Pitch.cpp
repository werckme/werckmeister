#include "Pitch.h"
#include <compiler/error.hpp>
#include <com/midi.hpp>

namespace documentModel
{
    namespace conductor
    {
        void PitchDecl::perform(const Events &events) const
        {
            com::midi::Event* noteOn = events.noteOn;
            com::midi::Event* noteOff = events.noteOff;
            com::midi::Event* predecessorNoteOn = events.predecessorNoteOn;
            if (!noteOff) 
            {
                return;
            }
            FGetValue getOriginalValue = [noteOn]() { return noteOn->parameter1(); };
            FGetValue getPercentBase = [noteOn]() { return noteOn->parameter1(); };
            FSetValue setNoteOn = [](com::midi::Event* noteOn, double val) { noteOn->parameter1(com::Byte(val)); };
            FSetValue setNoteOff = [](com::midi::Event* noteOff, double val) { noteOff->parameter1(com::Byte(val)); };
            FGetOptionalValue getPredecessorValue = [predecessorNoteOn]() -> std::optional<double>
            {
                if (!predecessorNoteOn) 
                {
                    return std::nullopt;
                }
                return predecessorNoteOn->parameter1();
            };
            double inputValue = declaration.value;
            constexpr double min = 0.0;
            constexpr double max = double(com::midi::MaxMidiValue);
            performImpl(noteOn, noteOff, inputValue, min, max, getOriginalValue, getPercentBase, getPredecessorValue, setNoteOn, setNoteOff);
        }
    }
}