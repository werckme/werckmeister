#pragma once

#include "AMidiContextCommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="instrumentDef" where="document">
    /// Adds a new MIDI instrument.
    /// see [instrument](#instrument), [device](#device)
    /// #### example
    /// define an device, an instrument and assign it to a track.
    /// ```
    /// device: MyDevice midi 0;
    /// instrumentDef: organ _onDevice=MyDevice _ch=0 _pc=16; \n
    /// [ \n
    /// instrument: organ; \n
    /// { \n
    ///    c d e f \n
    /// } \n
    /// ] \n
    /// ```
    /// </command>
    /// <param name="setName"  position="0" type="text">An arbitary name.</param>
    /// <param name="onDevice" position="1" type="text">The device which to use (The name of the device, see [device](#device)).</param>
    /// <param name="ch"       position="2" type="0..15">The MIDI channel.</param>
    /// <param name="bankMsb"  position="3" type="0..127">A MIDI `bank select MSB` value.</param>
    /// <param name="pc"       position="4" type="0..127">A MIDI `program change` value.</param>
    /// <param name="bankLsb"  type="0..127">A MIDI `bank select LSB` value.</param>
    /// <param name="cc"       type="0..127" deprecated="use bankMsb instead">A MIDI `bank select MSB. Deprecated use bankMsb instead` value.</param>
    /// <param name="gmName"   type="text">A GM name. You can use a GM instrument name instead of a pc number. It is not necessary to give the full name, a subset will match. You can find a complete list of instrument names here: [GM Instruments](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/gm_instrument_names.md).</param>
    class DefineMidiInstrument : public AMidiContextCommand
    {
    public:
        static com::String NoGmName;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.InstrumentDef.WithName, 0),
            FM_PARAMETER_DEF(argumentNames.InstrumentDef.OnDevice, 1),
            FM_PARAMETER_DEF(argumentNames.InstrumentDef.Ch, 2),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.InstrumentDef.BankMsb, 3, -1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.InstrumentDef.Pc, 4, -1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.InstrumentDef.Cc, 5, -1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.InstrumentDef.BankLsb, 6, -1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.InstrumentDef.GmInstrument, 7, NoGmName),
        };
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
    };
}
