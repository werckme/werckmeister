#pragma once

#include "PerformerScript.h"
#include <functional>
#include <vector>

namespace lua
{
    class FluidWriterPerformer : public PerformerScript
    {
    public:
        typedef PerformerScript Base;
        typedef std::function<void(const unsigned char*, unsigned int)> SysexHandler;
        FluidWriterPerformer(com::midi::MidiPtr midiFile) : PerformerScript(midiFile) 
        {
        }
        virtual ~FluidWriterPerformer() = default;
        typedef std::function<void(const com::midi::Event*)> FSendInputEventToScript;
        typedef std::function<void(int)> FAllNotesOff;
        FSendInputEventToScript sendInputEventToScript = [](auto){};
        FAllNotesOff sendAllNotesOff = [](auto){};
        void sysexHandler(SysexHandler sysexHandler) { _sysexHandler = sysexHandler; };
        SysexHandler sysexHandler() const { return _sysexHandler; }
    protected:
        SysexHandler _sysexHandler;
        virtual LuaMidiInputs getLuaMidiInputs() override;
        virtual void initLuaFunctions(sol::state_view&) override;
        virtual void listenTo(const std::string &id, sol::protected_function callBack) override;
        virtual void sendNoteOffs() override;
        virtual void sendNoteOffs(int channel) override;
        virtual void updateNoteOnCache(const Output& output, const com::midi::Event*) override;
    private:
        void handleSysex(std::vector<int> ints);
    };
    typedef std::shared_ptr<FluidWriterPerformer> FluidWriterPerformerPtr;
}