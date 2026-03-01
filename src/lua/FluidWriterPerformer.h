#pragma once

#include "PerformerScript.h"

namespace lua
{
    class FluidWriterPerformer : public PerformerScript
    {
    public:
        FluidWriterPerformer(com::midi::MidiPtr midiFile) : PerformerScript(midiFile) 
        {
        }
        virtual ~FluidWriterPerformer() = default;
    };
    typedef std::shared_ptr<FluidWriterPerformer> FluidWriterPerformerPtr;
}