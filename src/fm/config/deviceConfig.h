#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <string>

namespace fm {
    struct DeviceConfig {
        enum Type {
            Undefinded,
            Midi
        };
        Type type;
        typedef std::string DeviceId;
        DeviceId deviceId;
    };
}

#endif