#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <unordered_map>
#include <string>
#include "deviceConfig.h"

namespace fmapp {
    class IConfigServer {
    public:
        virtual ~IConfigServer() = default;
        virtual void addDevice(const std::string &name, const DeviceConfig &config) = 0;
        virtual const DeviceConfig * getDevice(const std::string &name) const = 0;
    };

    class ConfigServer : public IConfigServer {
        virtual void addDevice(const std::string &name, const DeviceConfig &config) override;
        virtual const DeviceConfig * getDevice(const std::string &name) const override;
    private:
        typedef std::unordered_map<std::string, DeviceConfig> Devices;
        Devices devices;
    };
}
#endif