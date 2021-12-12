#pragma once

#include <com/IHasParameter.h>
#include <vector>
#include <documentModel/Argument.h>

namespace com
{
    class AConvertsArgumentsToParameter : public com::IHasParameter
    {
    public:
        typedef std::vector<documentModel::Argument> Arguments;
        virtual ~AConvertsArgumentsToParameter() = default;
        virtual void setArguments(const Arguments &args);
    };
}
