#include "AddDevice.h"
#include <compiler/context/IContext.h>
#include <compiler/error.hpp>
#include <com/config/configServer.h>
#include <com/werckmeister.hpp>

namespace documentModel {
    namespace compiler {
        void AddDevice::execute(IContextPtr  context)
        {
            auto name             = parameters[argumentNames.Device.WithName].value<com::String>();
            auto type             = parameters[argumentNames.Device.IsType].value<com::String>();
            int offsetMillis = 0;
            if (parameters[argumentNames.Device.OffsetIndicator].strValue() == "offset") {
                offsetMillis = parameters[argumentNames.Device.OffsetValue].value<int>();
            }
            if (parameters[argumentNames.Device.WithOffset].empty() == false) {
                offsetMillis = parameters[argumentNames.Device.WithOffset].value<int>();
            }
            if (type == "midi") {
                auto deviceId = parameters[argumentNames.Device.UsePort].value<com::String>();
                addMidiDevice(name, deviceId, offsetMillis);
                return;
            }
            if (type == "fluidSynth") {
                auto font = parameters[argumentNames.Device.UseFont].value<com::String>();
                addFluidSynthDevice(name, font, offsetMillis);
                return;
            }
            com::StringStream ss;
            ss << "device type: '" << type << "' is not supported.";
            FM_THROW(Exception, ss.str());

        }

        void AddDevice::addMidiDevice(const com::String& uname, const com::String& deviceId, int offsetMillis)
        {
            auto& cs = com::getConfigServer();
            auto device = cs.createMidiDeviceConfig(uname, deviceId, offsetMillis);
            cs.addDevice(device);
        }

        void AddDevice::addFluidSynthDevice(const com::String& uname, const com::String& soundfontPath, int offsetMillis)
        {
            auto& cs = com::getConfigServer();
            auto resolvedPath = com::getWerckmeister().resolvePath(soundfontPath);
            auto device = cs.createFluidSynthDeviceConfig(uname, resolvedPath, offsetMillis);
            cs.addDevice(device);
        }
    }
}