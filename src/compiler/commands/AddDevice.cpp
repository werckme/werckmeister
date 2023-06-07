#include "AddDevice.h"
#include <compiler/context/IContext.h>
#include <compiler/error.hpp>
#include <com/config/configServer.h>
#include <com/werckmeister.hpp>
#include <com/config.hpp>

namespace compiler
{
    com::String AddDevice::NoDeviceName = "NO_DEVICE_NAME";
    void AddDevice::execute(IContextPtr context)
    {
        auto name = parameters[argumentNames.Device.WithName].value<com::String>();
        auto type = parameters[argumentNames.Device.IsType].value<com::String>();
        int offsetMillis = 0;
        if (parameters[argumentNames.Device.OffsetIndicator].strValue() == "offset")
        {
            offsetMillis = parameters[argumentNames.Device.OffsetValue].value<int>();
        }
        if (parameters[argumentNames.Device.WithOffset].empty() == false)
        {
            offsetMillis = parameters[argumentNames.Device.WithOffset].value<int>();
        }
        if (type == "midi")
        {
            auto deviceId = parameters[argumentNames.Device.UsePort].value<int>();
            auto deviceName = parameters[argumentNames.Device.UseDevice].value<com::String>();
            if (deviceId == NO_DEVICE_ID && deviceName == NoDeviceName)
            {
                FM_THROW(Exception, "neither a port nor a device name was given");
            }
            if (deviceId != NO_DEVICE_ID)
            {
                addMidiDeviceById(name, deviceId, offsetMillis);
            }
            else 
            {
                addMidiDeviceByName(name, deviceName, offsetMillis);
            }
            return;
        }
        if (type == "fluidSynth")
        {
            auto font = parameters[argumentNames.Device.UseFont].value<com::String>();
            addFluidSynthDevice(name, font, offsetMillis);
            return;
        }
#if IS_EMSCRIPTEN_BUILD == 1
        if(type == "webPlayer")
        {
            auto repo = parameters[argumentNames.Device.UseFont].value<com::String>();
            addWebRepo(name, repo);
            return;
        }
#endif
        com::StringStream ss;
        ss << "device type: '" << type << "' is not supported.";
        FM_THROW(Exception, ss.str());
    }

    void AddDevice::addMidiDeviceById(const com::String &uname, int deviceId, int offsetMillis)
    {
        auto &cs = com::getConfigServer();
        auto device = cs.createMidiDeviceConfigByDeviceId(uname, std::to_string(deviceId), offsetMillis);
        cs.addDevice(device);
    }

    void AddDevice::addMidiDeviceByName(const com::String &uname, const com::String &deviceName, int offsetMillis)
    {
        auto &cs = com::getConfigServer();
        auto device = cs.createMidiDeviceConfigByDeviceName(uname, deviceName, offsetMillis);
        cs.addDevice(device);
    }

    void AddDevice::addFluidSynthDevice(const com::String &uname, const com::String &soundfontPath, int offsetMillis)
    {
        auto &cs = com::getConfigServer();
        auto resolvedPath = com::getWerckmeister().resolvePath(soundfontPath);
        auto device = cs.createFluidSynthDeviceConfig(uname, resolvedPath, offsetMillis);
        cs.addDevice(device);
    }

    void AddDevice::addWebRepo(const com::String &uname, const com::String &repoUrl)
    {
        auto &cs = com::getConfigServer();
        auto device = cs.createWebPlayerConfig(uname, repoUrl);
        cs.addDevice(device);
    }
}