#include "configServer.h"

namespace fmapp {
    void ConfigServer::addDevice(const std::string &name, const DeviceConfig &config)
    {
        devices[name] = config;
    }
    const DeviceConfig * ConfigServer::getDevice(const std::string &name) const
    {
        auto it = devices.find(name);
        if (it==devices.end()) {
            return nullptr;
        }
        return &(it->second);
    }
}