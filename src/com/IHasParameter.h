#pragma once

#include "Parameter.h"
#include <unordered_map>

#define FM_PARAMETER_DEF(name, position)                  \
    {                                                     \
        com::String(name), com::Parameter(name, position) \
    }
#define FM_PARAMETER_DEFAULT_DEF(name, position, defaultValue)          \
    {                                                                   \
        com::String(name), com::Parameter(name, position, defaultValue) \
    }
namespace com
{
    class IHasParameter
    {
    public:
        typedef com::String Name;
        class ParametersByNames : public std::unordered_map<Name, Parameter>
        {
        public:
            typedef std::unordered_map<Name, Parameter> Base;
            using Base::Base;
            Parameter &operator[](const Name &key)
            {
                auto it = this->find(key);
                if (it == this->end())
                {
                    Parameter newParameter(key, Parameter::UndefinedPosition);
                    it = this->emplace(std::make_pair(key, newParameter)).first;
                }
                return it->second;
            }
        };
        virtual ParametersByNames &getParameters() = 0;
        virtual ~IHasParameter() = default;
    };

}
