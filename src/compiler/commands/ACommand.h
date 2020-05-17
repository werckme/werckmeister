#ifndef SHEET_ACOMMAND_HPP
#define SHEET_ACOMMAND_HPP

#include <fm/IRegisterable.h>
#include <fm/IHasParameter.h>
#include <sheet/Argument.h>
#include <vector>

namespace sheet {
    namespace compiler {
        class AContext;
        class ACommand : public fm::IRegisterable, public fm::IHasParameter
        {
        public:
            typedef std::vector<Argument> Arguments;
            virtual ~ACommand() = default;
            virtual void execute(AContext*) = 0;
            virtual void setArguments(const Arguments &args);

        };
    }
}

#endif