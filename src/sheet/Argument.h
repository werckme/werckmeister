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

    namespace
    {
        template<typename TValue>
        TValue _convertStrValue(const fm::String strValue)
        {
            TValue result;
            fm::StringStream ss;
            ss << strValue;
            ss >> result;
            return result;
        }
        template<>
        fm::String _convertStrValue<fm::String>(const fm::String strValue)
        {
            return strValue;
        }
    }

    template<typename TValue>
    TValue Argument::parseValue() const
    {
        return _convertStrValue(this->value);
    }

}

#endif