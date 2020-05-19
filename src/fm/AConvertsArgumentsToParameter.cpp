#include "AConvertsArgumentsToParameter.h"
#include <fm/tools.h>

namespace fm {

    void AConvertsArgumentsToParameter::setArguments(const Arguments &args)
    {
        fm::argumentsToParameters(args, getParameters());
    }

}