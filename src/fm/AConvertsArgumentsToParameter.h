#ifndef SHEET_ACONVERTSARGUMENTSTOPARAMETER_HPP
#define SHEET_ACONVERTSARGUMENTSTOPARAMETER_HPP


#include <fm/IHasParameter.h>
#include <vector>
#include <sheet/Argument.h>

namespace fm {
    class AConvertsArgumentsToParameter : public fm::IHasParameter
    {
    public:
        typedef std::vector<sheet::Argument> Arguments;
        virtual ~AConvertsArgumentsToParameter() = default;
        virtual void setArguments(const Arguments &args);
    };
}

#endif