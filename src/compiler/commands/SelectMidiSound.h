#ifndef SELECTMIDISOUND_HPP
#define SELECTMIDISOUND_HPP

#include "AMidiContextCommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class SelectMidiSound : public AMidiContextCommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEFAULT_DEF		    (argumentNames.SelectMidiSound.CC, 	0, "0"),
                FM_PARAMETER_DEF	           	    (argumentNames.SelectMidiSound.PC, 	1)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif