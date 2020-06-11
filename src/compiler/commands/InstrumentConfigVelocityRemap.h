#ifndef INSTRUMENTCONFIGVELOCITYREMAP_HPP
#define INSTRUMENTCONFIGVELOCITYREMAP_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <compiler/commands/ACanSpecifyInstrument.h>

namespace sheet {
    namespace compiler {
        class InstrumentConfigVelocityRemap : public ACommand, public ACanSpecifyInstrument
        {
        public:
            static const fm::String VelocityNotSet;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.PPPPP, 	0, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.PPPP , 	1, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.PPP  , 	2, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.PP   , 	3, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.P    , 	4, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.F    , 	5, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.FF   , 	6, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.FFF  , 	7, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.FFFF , 	8, VelocityNotSet),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.VelocityRemap.FFFFF, 	9, VelocityNotSet),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContext*);
        };
    }
}

#endif