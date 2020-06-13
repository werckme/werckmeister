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
            auto deviceId         = parameters[argumentNames.Device.UsePort].value<fm::String>();
            int offsetMillis = 0;
            if (parameters[argumentNames.Device.OffsetIndicator].strValue() == "offset") {
                offsetMillis = parameters[argumentNames.Device.OffsetValue].value<int>();
            }
            if (parameters[argumentNames.Device.WithOffset].empty() == false) {
                offsetMillis = parameters[argumentNames.Device.WithOffset].value<int>();
            }
            if (type != "midi") {
                fm::StringStream ss;
                ss << "device type: '" << type << "' is not supported.";
                FM_THROW(Exception, ss.str());
            }
            auto &cs = fm::getConfigServer();
            auto device = cs.createMidiDeviceConfig(name, deviceId, offsetMillis);
            cs.addDevice(device);
        }
    }
}