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
        /// define an device, an instrument and assign it to a track.
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
        /// ``` \n
        /// **Fluidsynth**
        /// If you want to use [fluidSynth](http://www.fluidsynth.org) as output device, you can achieve this with a command like this:
        /// `device: MyDevice fluidSynth _useFont="PATH OF A SOUNDFONT FILE";`
        /// On mac, make sure that you have fluidsynth installed.
        /// </command>
        /// <param name="setName"    position="0" type="text">An arbitary name.</param>
        /// <param name="isType"     position="1" type="[midi,fluidSynth]">The type of the device.</param>
        /// <param name="usePort"    position="2" type="0..N">The midi port id of your device. You can get a list of your connected devices, by executing `sheetp --list`</param>
        /// <param name="withOffset" position=""  optional="1" type="0..N">Defines an offset in milliseconds. Can be used to keep different devices in sync.</param>
        /// <param name="useFont"    position=""  optional="1" type="a file path">Only valid if isType=`fluidsynth`. Sets the location of the soundfont file, which will be used by FluidSynth</param>
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
                FM_PARAMETER_DEF            (argumentNames.Device.UseFont,              6),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        private:
            void addMidiDevice(const fm::String& uname, const fm::String& deviceId, int offsetMillis);
            void addFluidSynthDevice(const fm::String& uname, const fm::String &soundfontPath, int offsetMillis);
        };
    }
}

#endif