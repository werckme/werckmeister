#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace compiler
{
    /// <command name="device" where="document">
    ///     Defines a device which can be used when adding instruments (see [instrumentDef](#instrumentDef))
    /// see [instrumentDef](#instrumentDef), [instrument](#instrument)
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
    /// ``` \n
    /// **Fluidsynth**
    /// If you want to use [fluidSynth](http://www.fluidsynth.org) as output device, you can achieve this with a command like this:
    /// `device: MyDevice fluidSynth _useFont="PATH OF A SOUNDFONT FILE";`
    /// On mac and linux, make sure that you have fluidsynth installed.
    /// If you are using multiple sound fonts with multiple devices, make sure to use the program change argument “_pc” in the instrument definition, otherwise no
    /// soundfont change will be triggered internally. 
    /// You can set an environment variable `WM_SOUNDFONT_PATH` to add a search path for soundfont files. *(only one path supported)*
    /// </command>
    /// <param name="setName"    position="0" type="text">An arbitary name.</param>
    /// <param name="isType"     position="1" type="[midi,fluidSynth]">The type of the device.</param>
    /// <param name="usePort"    position="2" optional="1" type="0..N">The midi port id of your device. You can get a list of your connected devices, by executing `sheetp --list`. Alternatively you can use `useDevice` instead.</param>
    /// <param name="useDevice"   position=""  optional="1" type="text">The name of the midi port of your device. You can get a list of your connected devices, by executing `sheetp --list`. It is not necessary to give the full name, a subset will match.</param>
    /// <param name="withOffset" position=""  optional="1" type="0..N">Defines an offset in milliseconds. Can be used to keep different devices in sync.</param>
    /// <param name="useFont"    position=""  optional="1" type="a file path">Only valid if isType=`fluidsynth`. Sets the location of the soundfont file, which will be used by FluidSynth</param>
    class AddDevice : public ACommand
    {
    public:
        enum DeviceIdConstants
        {
            NO_DEVICE_ID = -1
        };
        static com::String NoDeviceName;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.Device.WithName, 0),
            FM_PARAMETER_DEF(argumentNames.Device.IsType, 1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.Device.UsePort, 2, NO_DEVICE_ID),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.Device.UseDevice, 3, NoDeviceName),
            FM_PARAMETER_DEF(argumentNames.Device.OffsetIndicator, 4),
            FM_PARAMETER_DEF(argumentNames.Device.OffsetValue, 5),
            FM_PARAMETER_DEF(argumentNames.Device.WithOffset, 6),
            FM_PARAMETER_DEF(argumentNames.Device.UseFont, 7)
        };
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);

    private:
        void addMidiDeviceById(const com::String &uname, int deviceId, int offsetMillis);
        void addMidiDeviceByName(const com::String &uname, const com::String &deviceId, int offsetMillis);
        void addFluidSynthDevice(const com::String &uname, const com::String &soundfontPath, int offsetMillis);
        void addWebRepo(const com::String &uname, const com::String &repoUrl);
    };
}
