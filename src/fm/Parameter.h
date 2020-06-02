#ifndef FM_PARAMETER_H
#define FM_PARAMETER_H

#include "common.hpp"
#include "exception.hpp"
namespace fm
{
    class Parameter {
    public:
        enum { UndefinedPosition = -1 };
        Parameter() {}
        template<typename TValue>
        Parameter (const fm::String &name, int position, const TValue &defaultValue) : _name(name),
            _position(position) 
        {
            value<TValue>(defaultValue);
        }
        Parameter (const fm::String &name, int position) : _name(name), _position(position) {}
        /**
         * @return a parsed value
         * @throw if value is empty
         */
        template<typename TValue>
        TValue value() const;

        template<>
        fm::String value<fm::String>() const { return strValue(); }

        template<typename TValue>
        void value(const TValue& value);
        void strValue(const fm::String value) { this->_value = value; }
        /**
         * @return the raw string value
         */
        fm::String strValue() const { return this->_value; }
        const fm::String & name() const { return _name; }
        int position() const { return _position; }
        bool empty() const { return _value.empty(); }
    private:
        fm::String _name;
        fm::String _value;
        int _position = UndefinedPosition;
    };
    ///////////////////////////////////////////////////////////////////////////
    template<typename TValue>
    TValue Parameter::value() const
    { 
        if (_value.empty()) {
            throw Exception(fm::String("missing value for: " + name() ));
        }
        TValue result;
        fm::StringStream ss;
        ss << this->_value;
        ss >> result;
        return result;
    }

    template<typename TValue>
    void Parameter::value(const TValue& value)
    {
        fm::StringStream ss;
        ss << value;
        this->_value = ss.str();
    }
}


#endif