#include "configServer.h"
#include <com/exception.hpp>
#include <com/tools.h>

namespace com {
    
    ConfigServer & getConfigServer()
    {
        static ConfigServer instance;
		return instance;
    }
    
    void ConfigServer::addDevice(const DeviceConfig &config)
    {
        devices[config.name] = config;
    }
    const DeviceConfig * ConfigServer::getDevice(const com::String &name) const
    {
        auto it = devices.find(name);
        if (it==devices.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    DeviceConfig ConfigServer::createMidiDeviceConfig(const com::String &uname, const DeviceConfig::DeviceId &deviceId, int offsetMillis) 
    {
        DeviceConfig cf;
        cf.name = uname;
        cf.type = DeviceConfig::Midi;
        cf.deviceId = deviceId;
        cf.offsetMillis = offsetMillis;
        return cf;
    }

    DeviceConfig ConfigServer::createFluidSynthDeviceConfig(const com::String& uname, const std::string& soundfontFile, int offsetMillis)
    {
        DeviceConfig cf;
        cf.name = uname;
        cf.type = DeviceConfig::FluidSynth;
        cf.offsetMillis = offsetMillis;
        cf.deviceId = soundfontFile;
        return cf;
    }

    void ConfigServer::clear()
    {
        this->devices.clear();
    }

    ConfigServer::~ConfigServer() = default;
    ConfigServer::ConfigServer() = default;
}