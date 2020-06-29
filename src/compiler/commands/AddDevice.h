#ifndef ADDDEVICE_HPP
#define ADDDEVICE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="device" where="document">
        ///     Defines a device which can be used when adding instruments (see [instrumentDef](#instrumentDef))
        /// ### examples
        /// **positional:** 
        /// `device: MyDevice midi 0 offset 100;`
        /// **named:**
        /// `device: _setName=MyDevice _isType=midi _usePort=0 _withOffset=100;`   
        /// **a complete example**
        /// define an device, an instrument and set it to a track.
        /// see [instrumentDef](#instrumentDef), [instrument](#instrument)
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
        /// <param name="setName"    position="0" range="[a-zA-Z0-9.]+">An arbitary name.</param>
        /// <param name="isType"     position="1" range="[midi]">The type of the device. (Currently the only supported type is `midi`)</param>
        /// <param name="usePort"    position="2" range="0..N">The port id of your device. You can get a list of your connected devices, by executing `sheetp --list`</param>
        /// <param name="withOffset" position=""  optional="1" range="0..N">Defines an offset in milliseconds. Can be used to keep different devices in sync.</param>
        class AddDevice : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.Device.WithName, 	        0),
                FM_PARAMETER_DEF		    (argumentNames.Device.IsType, 	            1),
                FM_PARAMETER_DEF		    (argumentNames.Device.UsePort, 	            2),
                FM_PARAMETER_DEF            (argumentNames.Device.OffsetIndicator, 	    3),
                FM_PARAMETER_DEF            (argumentNames.Device.OffsetValue, 	        4),
                FM_PARAMETER_DEF            (argumentNames.Device.WithOffset, 	        5),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif