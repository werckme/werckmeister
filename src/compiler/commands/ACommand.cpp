#include "ACommand.h"
#include <fm/tools.h>

namespace sheet {
    namespace compiler {
        void ACommand::setArguments(const Arguments &args)
        {
            fm::argumentsToParameters(args, getParameters());
        }
    }
}