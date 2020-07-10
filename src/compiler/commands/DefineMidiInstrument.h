#ifndef SHEET_DEFINEMIDIINSTRUMENT_HPP
#define SHEET_DEFINEMIDIINSTRUMENT_HPP

#include "AMidiContextCommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="instrumentDef" where="document">
        /// Adds a new MIDI instrument.
        /// ### examples
        /// **positional:** 
        /// `instrumentDef: drums MyDevice 9 0 3;`
        /// **named:**
        /// `instrumentDef: _setName=drums _onDevice=MyDevice _ch=9 _cc=0 _pc=3;`
        /// **a complete example**
        /// define an device, an instrument and set it to a track.
        /// see [instrument](#instrument), [device](#device)
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
        /// <param name="setName"  position="0" range="[a-zA-Z0-9.]+">An arbitary name.</param>
        /// <param name="onDevice" position="1" range="[a-zA-Z0-9.]+">The device which to use (The name of the device, see [device](#device)).</param>
        /// <param name="ch"       position="2" range="0..15">The MIDI channel.</param>
        /// <param name="cc"       position="3" range="0..127">A MIDI `control change` value.</param>
        /// <param name="pc"       position="4" range="0..127">A MIDI `program change` value.</param>
        class DefineMidiInstrument : public AMidiContextCommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.WithName, 	0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.OnDevice, 	1),
                FM_PARAMETER_DEF	        (argumentNames.InstrumentDef.Ch, 	    2),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.InstrumentDef.Cc, 	    3,  0),
                FM_PARAMETER_DEF		    (argumentNames.InstrumentDef.Pc, 	    4),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif