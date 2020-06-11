#ifndef SHEET_ACOMMAND_HPP
#define SHEET_ACOMMAND_HPP

#include <fm/IRegisterable.h>
#include <fm/AConvertsArgumentsToParameter.h>
#include <sheet/Argument.h>
#include <vector>

namespace sheet {
    namespace compiler {
        class IContext;
        class ACommand : public fm::IRegisterable, public fm::AConvertsArgumentsToParameter
        {
        public:
            typedef std::vector<Argument> Arguments;
            virtual ~ACommand() = default;
            virtual void execute(IContext*) = 0;
            
        };
    }
}

#endif