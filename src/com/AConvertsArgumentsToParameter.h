#ifndef SHEET_ACONVERTSARGUMENTSTOPARAMETER_HPP
#define SHEET_ACONVERTSARGUMENTSTOPARAMETER_HPP


#include <com/IHasParameter.h>
#include <vector>
#include <sheet/Argument.h>

namespace com {
    class AConvertsArgumentsToParameter : public com::IHasParameter
    {
    public:
        typedef std::vector<sheet::Argument> Arguments;
        virtual ~AConvertsArgumentsToParameter() = default;
        virtual void setArguments(const Arguments &args);
    };
}

#endif