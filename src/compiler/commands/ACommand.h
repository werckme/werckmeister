#pragma once

#include <com/IRegisterable.h>
#include <com/AConvertsArgumentsToParameter.h>
#include <documentModel/Argument.h>
#include <vector>
#include <compiler/context/IContext.h>

namespace documentModel {
    namespace compiler {
        class ACommand : public com::IRegisterable, public com::AConvertsArgumentsToParameter
        {
        public:
            typedef std::vector<Argument> Arguments;
            virtual ~ACommand() = default;
            virtual void execute(IContextPtr ) = 0;
            
        };
    }
}

