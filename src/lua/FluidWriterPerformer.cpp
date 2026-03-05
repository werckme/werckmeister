#include "FluidWriterPerformer.h"
#include <iostream>

namespace
{
    const com::String DummyInputId = "libwerckmeister.01";
}

namespace lua
{
    PerformerScript::LuaMidiInputs FluidWriterPerformer::getLuaMidiInputs()
    {
        LuaMidiInput luaIn;
        luaIn.id = DummyInputId;
        luaIn.name = DummyInputId;
        return LuaMidiInputs({luaIn});
    }

    void FluidWriterPerformer::listenTo(const std::string &id, sol::protected_function callBack)
    {
        if (id != DummyInputId)
        {
            throw std::runtime_error("invalid midi input id: " + id);
        }
        sendInputEventToScript = [this, callBack](const com::midi::Event *midiEvent)
        {
            auto luaMidiEvent = createLuaMidiFrom(*midiEvent);
            auto result = callBack(const_cast<LuaMidi*>(&luaMidiEvent));
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Error: " << err.what() << std::endl;
            }
        };
    }
    void FluidWriterPerformer::sendNoteOffs() 
    {
        for(int i=0; i<15; ++i)
        {
            sendNoteOffs(i);
        }
    }
    void FluidWriterPerformer::sendNoteOffs(int channel) 
    {
        sendAllNotesOff(channel);
    }
    void FluidWriterPerformer::updateNoteOnCache(const Output& output, const com::midi::Event*) 
    {
    }
}