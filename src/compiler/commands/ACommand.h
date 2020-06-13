#ifndef SHEET_ACOMMAND_HPP
#define SHEET_ACOMMAND_HPP

#include <fm/IRegisterable.h>
#include <fm/AConvertsArgumentsToParameter.h>
#include <sheet/Argument.h>
#include <vector>
#include <compiler/context/IContext.h>

namespace sheet {
    namespace compiler {
        class ACommand : public fm::IRegisterable, public fm::AConvertsArgumentsToParameter
        {
        public:
            typedef std::vector<Argument> Arguments;
            virtual ~ACommand() = default;
            virtual void execute(IContextPtr ) = 0;
            
        };
    }
}

#endif