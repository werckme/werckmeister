#include "AConvertsArgumentsToParameter.h"
#include <com/tools.h>

namespace com
{

    void AConvertsArgumentsToParameter::setArguments(const Arguments &args)
    {
        com::argumentsToParameters(args, getParameters());
    }

}