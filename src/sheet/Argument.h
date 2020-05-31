#ifndef SHEET_ARGUMENT_H
#define SHEET_ARGUMENT_H

#include <fm/common.hpp>

namespace sheet {

    struct Argument
    {
        fm::String value;
        fm::String name;

        template<typename TValue>
        TValue parseValue() const;
    };

    template<typename TValue>
    TValue Argument::parseValue() const
    {
        TValue result;
        fm::StringStream ss;
        ss << this->value;
        ss >> result;
        return result;
    }

}

#endif