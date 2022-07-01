#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "deviceConfig.h"
#include <com/common.hpp>

namespace com
{
    class ConfigServer
    {
    public:
        typedef std::unordered_map<com::String, DeviceConfig> Devices;
        ConfigServer();
        ConfigServer(const ConfigServer &&) = delete;
        ConfigServer &operator=(const ConfigServer &&) = delete;
        ~ConfigServer();
        DeviceConfig createWebPlayerConfig(const com::String &uname, const std::string &repoUrl);
        DeviceConfig createMidiDeviceConfig(const com::String &uname, const DeviceConfig::DeviceId &deviceId, int offsetMillis = 0);
        DeviceConfig createFluidSynthDeviceConfig(const com::String &uname, const std::string &soundfontFile, int offsetMillis = 0);
        void addDevice(const DeviceConfig &config);
        void clear();
        const DeviceConfig *getDevice(const com::String &name) const;
        const Devices &getDevices() const { return devices; }

    private:
        Devices devices;
    };
    ConfigServer &getConfigServer();
}
