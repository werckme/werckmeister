#ifndef FM_IHAS_PARAMETER_H
#define FM_IHAS_PARAMETER_H

#include "Parameter.h"
#include <unordered_map>

#define FM_PARAMETER_DEF(name, position) { fm::String(name), fm::Parameter(name, position) }
#define FM_PARAMETER_DEFAULT_DEF(name, position, defaultValue) { fm::String(name), fm::Parameter(name, position, defaultValue) }
namespace fm
{
    class IHasParameter {
    public:
        typedef fm::String Name;
        class ParametersByNames : public std::unordered_map<Name, Parameter>
        {
        public:
            typedef std::unordered_map<Name, Parameter> Base;
            using Base::Base;
            Parameter& operator[] (const Name& key) 
            {
                auto it = this->find(key);
                if (it == this->end()) {
                    Parameter newParameter(key, Parameter::UndefinedPosition);
                    it = this->emplace(std::make_pair(key, newParameter)).first;
                }
                return it->second;
            } 
        };
        virtual ParametersByNames & getParameters() const = 0;
        virtual ~IHasParameter() = default;
    };

}


#endif