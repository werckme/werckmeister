#include "Channel.h"
#include <regex>
#include <string>
#include <iostream> 
#include <compiler/commands/SetCC.h>
#include <charconv> 

namespace conductor
{


    SetChannel::SetChannel() 
    {
    }

    void SetChannel::perform(const Events &events) const
    {
        com::midi::Event *noteOn = events.midiEvent;
        com::midi::Event *noteOff = events.noteOff;
        int channel = (int)declaration.numberValue();
        if (channel < 0 || channel > 15)
        {
            FM_THROW(compiler::Exception, "invalid channel number: " + std::to_string(channel));
        }
        switch (declaration.unit)
        {
            case documentModel::ConductionRule::Declaration::UnitPercent:
                FM_THROW(compiler::Exception, "invalid unit type: %");
            default: {}
        }
        noteOn->channel(channel);
        noteOff->channel(channel);
    }
}