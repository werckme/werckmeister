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
        typedef std::function<void(const com::midi::Event*)> FSendInputEventToScript;
        FSendInputEventToScript sendInputEventToScript = [](auto ev){};
    protected:
        virtual LuaMidiInputs getLuaMidiInputs() override;
        virtual void listenTo(const std::string &id, sol::protected_function callBack) override;
    };
    typedef std::shared_ptr<FluidWriterPerformer> FluidWriterPerformerPtr;
}