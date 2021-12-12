#pragma once

#include <com/common.hpp>

namespace documentModel
{

    struct Argument
    {
        com::String value;
        com::String name;

        template <typename TValue>
        TValue parseValue() const;
    };

    namespace
    {
        template <typename TValue>
        TValue _convertStrValue(const com::String strValue)
        {
            TValue result;
            com::StringStream ss;
            ss << strValue;
            ss >> result;
            return result;
        }
        template <>
        com::String _convertStrValue<com::String>(const com::String strValue)
        {
            return strValue;
        }
    }

    template <typename TValue>
    TValue Argument::parseValue() const
    {
        return _convertStrValue<TValue>(this->value);
    }

}
