#pragma once

#include "PerformerScript.h"

namespace lua
{
    class FluidWriterPerformer : public PerformerScript
    {
    public:
        typedef PerformerScript Base;
        FluidWriterPerformer(com::midi::MidiPtr midiFile) : PerformerScript(midiFile) 
        {
        }
        virtual ~FluidWriterPerformer() = default;
        typedef std::function<void(const com::midi::Event*)> FSendInputEventToScript;
        typedef std::function<void(int)> FAllNotesOff;
        FSendInputEventToScript sendInputEventToScript = [](auto ev){};
        FAllNotesOff sendAllNotesOff = [](auto ch){};
    protected:
        virtual LuaMidiInputs getLuaMidiInputs() override;
        virtual void initLuaFunctions(sol::state_view&) override;
        virtual void listenTo(const std::string &id, sol::protected_function callBack) override;
        virtual void sendNoteOffs() override;
        virtual void sendNoteOffs(int channel) override;
        virtual void updateNoteOnCache(const Output& output, const com::midi::Event*) override;
    };
    typedef std::shared_ptr<FluidWriterPerformer> FluidWriterPerformerPtr;
}