#include "configServer.h"
#include <exception>

namespace fm {
    
    ConfigServer & getConfigServer()
    {
        typedef Loki::SingletonHolder<ConfigServer> Holder;
		return Holder::Instance();
    }
    
    void ConfigServer::addDevice(const fm::String &name, const DeviceConfig &config)
    {
        devices[name] = config;
    }
    const DeviceConfig * ConfigServer::getDevice(const fm::String &name) const
    {
        auto it = devices.find(name);
        if (it==devices.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    DeviceConfig ConfigServer::createDeviceConfig(const fm::String &name, std::vector<fm::String> &args) 
    {
        DeviceConfig cf;
        if (args.empty()) {
            throw std::runtime_error("missing device type argument");
        }
        if (args.at(0) == FM_STRING("midi")) {
            if (args.size() < 2) {
                throw std::runtime_error("missing deviceid argument");
            }
            cf.type = DeviceConfig::Midi;
            cf.deviceId = fm::to_string(args.at(1));
        }
        return cf;
    }

    ConfigServer::~ConfigServer() = default;
    ConfigServer::ConfigServer() = default;
}