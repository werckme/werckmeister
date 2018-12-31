#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <unordered_map>
#include <string>
#include "IConfigServer.h"


#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <loki/Singleton.h>
#pragma GCC diagnostic pop
#else
#include <loki/Singleton.h>
#endif

namespace fm {
    class ConfigServer : public IConfigServer {
    friend struct Loki::CreateUsingNew<ConfigServer>;
    public:
        ConfigServer();
		ConfigServer(const ConfigServer&&) = delete;
		ConfigServer& operator=(const ConfigServer&&) = delete;
        ~ConfigServer();
        virtual DeviceConfig createDeviceConfig(const fm::String &name, std::vector<fm::String> &args) override;
        virtual void addDevice(const fm::String &name, const DeviceConfig &config) override;
        virtual const DeviceConfig * getDevice(const fm::String &name) const override;
    private:
        typedef std::unordered_map<fm::String, DeviceConfig> Devices;
        Devices devices;
    };
    ConfigServer & getConfigServer();
}
#endif