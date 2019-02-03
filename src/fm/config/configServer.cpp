#include "configServer.h"
#include <fm/exception.hpp>

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
            FM_THROW(Exception, "missing device type argument");
        }
        auto type = args.at(0);
        if (type == FM_STRING("midi")) {
            if (args.size() < 2) {
                FM_THROW(Exception, "missing deviceid argument");
            }
            cf.type = DeviceConfig::Midi;
            cf.deviceId = fm::to_string(args.at(1));
        }
        if (cf.type == DeviceConfig::Undefinded) {
            FM_THROW(Exception, "no config for " + fm::to_string(name) + ", " + fm::to_string(type));
        }
        return cf;
    }

    ConfigServer::~ConfigServer() = default;
    ConfigServer::ConfigServer() = default;
}