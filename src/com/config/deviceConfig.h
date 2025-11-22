#pragma once

#include <string>

namespace com
{
    struct DeviceConfig
    {
        enum Type
        {
            Undefinded,
            Midi,
            FluidSynth,
            WebPlayer
        };
        std::string name; // the name of the device
        int offsetMillis = 0;
        Type type = Undefinded;
        typedef std::string DeviceId;
        DeviceId deviceId;
        std::string deviceName; // bad naming here, whats the differnce to name? (⌣_⌣”)
                                // deviceName means for e.g. the name of the midi port e.g. "SC 1"
    };
}
