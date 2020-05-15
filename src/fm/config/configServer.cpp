#include "configServer.h"
#include <fm/exception.hpp>
#include <fm/tools.h>
#include <compiler/argumentNames.h>

namespace fm {
    
    ConfigServer & getConfigServer()
    {
        typedef Loki::SingletonHolder<ConfigServer> Holder;
		return Holder::Instance();
    }
    
    void ConfigServer::addDevice(const DeviceConfig &config)
    {
        devices[config.name] = config;
    }
    const DeviceConfig * ConfigServer::getDevice(const fm::String &name) const
    {
        auto it = devices.find(name);
        if (it==devices.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    DeviceConfig ConfigServer::createDeviceConfig(const std::vector<sheet::Argument> &args) 
    {
        DeviceConfig cf;
        cf.name = sheet::getArgValue<fm::String>(args, argumentNames.Device.WithName, 0);
        auto type = sheet::getArgValue<fm::String>(args, argumentNames.Device.IsType, 1);
        if (type == FM_STRING("midi")) {
            cf.type = DeviceConfig::Midi;
            cf.deviceId = sheet::getArgValue<fm::String>(args, argumentNames.Device.UsePort, 2);
        }
        auto offsetValue = sheet::getArgValue<int>(args, argumentNames.Device.Offset, sheet::NO_ARG_POSITION, -1);
        cf.offsetMillis = offsetValue >0 ? offsetValue : 0;
        if (cf.type == DeviceConfig::Undefinded) {
            FM_THROW(Exception, "no config for " + cf.name + ", " + type);
        }
        return cf;
    }

    void ConfigServer::clear()
    {
        this->devices.clear();
    }

    ConfigServer::~ConfigServer() = default;
    ConfigServer::ConfigServer() = default;
}