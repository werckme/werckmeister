#include "DefineMidiInstrument.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>
#include <com/GmInstrumentNames.h>
#include <tuple>

namespace compiler
{
    namespace
    {
        int findProgramChangeNumber(com::String gmName, int fallbackPc)
        {
            for(const auto &namePcTuple : com::gmInstruments)
            {
                auto name = std::get<0>(namePcTuple);
                bool match = name.find(gmName) != com::String::npos;
                if (match) 
                {
                    return std::get<1>(namePcTuple);
                }
            }
            return fallbackPc;
        }
    }
    com::String DefineMidiInstrument::NoGmName = "NO_GM_NAME";
    void DefineMidiInstrument::execute(IContextPtr context)
    {
        auto midiContext = getMidiContext(context);
        auto name = parameters[argumentNames.InstrumentDef.WithName].value<com::String>();
        auto deviceName = parameters[argumentNames.InstrumentDef.OnDevice].value<com::String>();
        auto ch = parameters[argumentNames.InstrumentDef.Ch].value<int>();
        auto bankMsb = parameters[argumentNames.InstrumentDef.BankMsb].value<int>();
        auto bankLsb = parameters[argumentNames.InstrumentDef.BankLsb].value<int>();
        auto pc = parameters[argumentNames.InstrumentDef.Pc].value<int>();
        auto gmName = parameters[argumentNames.InstrumentDef.GmInstrument].value<com::String>();
        if (gmName != NoGmName)
        {
            pc = findProgramChangeNumber(gmName, pc);
        }
        if (bankMsb < 0)
        {
            bankMsb = parameters[argumentNames.InstrumentDef.Cc].value<int>();
        }
        midiContext->defineMidiInstrument(name, deviceName, ch, bankMsb, bankLsb, pc);
    }
}