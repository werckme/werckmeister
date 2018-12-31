#ifndef ICONFIGSERVER_H
#define ICONFIGSERVER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "deviceConfig.h"
#include <fm/common.hpp>

namespace fm {
    class IConfigServer {
    public:
        virtual ~IConfigServer() = default;
        virtual DeviceConfig createDeviceConfig(const fm::String &name, std::vector<fm::String> &args) = 0;
        virtual void addDevice(const fm::String &name, const DeviceConfig &config) = 0;
        virtual const DeviceConfig * getDevice(const fm::String &name) const = 0;
    };
}
#endif