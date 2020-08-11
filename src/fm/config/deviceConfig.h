#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <string>

namespace fm {
    struct DeviceConfig {
        enum Type {
            Undefinded,
            Midi,
            FluidSynth
        };
        std::string name;
        int offsetMillis = 0;
        Type type = Undefinded;
        typedef std::string DeviceId;
        DeviceId deviceId;
    };
}

#endif