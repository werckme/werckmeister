#ifndef $GUARD_NAME
#define $GUARD_NAME

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        class $NAME : public ACommand
        {
        public:
            com::IHasParameter::ParametersByNames parameters = {
                //FM_PARAMETER_DEF		    (argumentNames.XYZ, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif