#include "configServer.h"
#include <fm/exception.hpp>
#include <sheet/tools.h>

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
    DeviceConfig ConfigServer::createDeviceConfig(const fm::String &name, std::vector<sheet::Argument> &args) 
    {
        // #74.2 TODO
        DeviceConfig cf;
        if (args.empty()) {
            FM_THROW(Exception, "missing device type argument");
        }
        auto type = args.at(0);
        if (type.value == FM_STRING("midi")) {
            if (args.size() < 2) {
                FM_THROW(Exception, "missing deviceid argument");
            }
            cf.type = DeviceConfig::Midi;
            cf.deviceId = args.at(1).value;
        }
        auto offsetValue = sheet::getArgValueFor<int>(FM_STRING("offset"), args);
        if (offsetValue.first) { // offset in ms
            cf.offsetMillis = offsetValue.second;
        }
        if (cf.type == DeviceConfig::Undefinded) {
            FM_THROW(Exception, "no config for " + name + ", " + type.value);
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