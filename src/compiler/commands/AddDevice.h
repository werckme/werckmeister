#ifndef ADDDEVICE_HPP
#define ADDDEVICE_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class AddDevice : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.Device.WithName, 	        0),
                FM_PARAMETER_DEF		    (argumentNames.Device.IsType, 	            1),
                FM_PARAMETER_DEF		    (argumentNames.Device.UsePort, 	            2),
                FM_PARAMETER_DEF            (argumentNames.Device.OffsetIndicator, 	    3),
                FM_PARAMETER_DEF            (argumentNames.Device.OffsetValue, 	        4),
                FM_PARAMETER_DEF            (argumentNames.Device.WithOffset, 	        5),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
        };
    }
}

#endif