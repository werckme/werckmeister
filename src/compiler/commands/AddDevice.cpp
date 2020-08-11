#include "AddDevice.h"
#include <compiler/context/IContext.h>
#include <compiler/error.hpp>
#include <fm/config/configServer.h>

namespace sheet {
    namespace compiler {
        void AddDevice::execute(IContextPtr  context)
        {
            auto name             = parameters[argumentNames.Device.WithName].value<fm::String>();
            auto type             = parameters[argumentNames.Device.IsType].value<fm::String>();
            int offsetMillis = 0;
            if (parameters[argumentNames.Device.OffsetIndicator].strValue() == "offset") {
                offsetMillis = parameters[argumentNames.Device.OffsetValue].value<int>();
            }
            if (parameters[argumentNames.Device.WithOffset].empty() == false) {
                offsetMillis = parameters[argumentNames.Device.WithOffset].value<int>();
            }
            if (type == "midi") {
                auto deviceId = parameters[argumentNames.Device.UsePort].value<fm::String>();
                addMidiDevice(name, deviceId, offsetMillis);
                return;
            }
            if (type == "fluidSynth") {
                auto font = parameters[argumentNames.Device.UseFont].value<fm::String>();
                addFluidSynthDevice(name, font, offsetMillis);
                return;
            }
            fm::StringStream ss;
            ss << "device type: '" << type << "' is not supported.";
            FM_THROW(Exception, ss.str());

        }

        void AddDevice::addMidiDevice(const fm::String& uname, const fm::String& deviceId, int offsetMillis)
        {
            auto& cs = fm::getConfigServer();
            auto device = cs.createMidiDeviceConfig(uname, deviceId, offsetMillis);
            cs.addDevice(device);
        }

        void AddDevice::addFluidSynthDevice(const fm::String& uname, const fm::String& soundfontPath, int offsetMillis)
        {
            auto& cs = fm::getConfigServer();
            auto device = cs.createFluidSynthDeviceConfig(uname, soundfontPath, offsetMillis);
            cs.addDevice(device);
        }
    }
}