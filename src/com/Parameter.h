#pragma once

#include "common.hpp"
#include "exception.hpp"
namespace com
{
    class Parameter
    {
    public:
        enum
        {
            UndefinedPosition = -1
        };
        Parameter() {}
        template <typename TValue>
        Parameter(const com::String &name, int position, const TValue &defaultValue) : _name(name),
                                                                                       _position(position)
        {
            value<TValue>(defaultValue);
        }
        Parameter(const com::String &name, int position) : _name(name), _position(position) {}
        /**
         * @return a parsed value
         * @throw if value is empty
         */
        template <typename TValue>
        TValue value() const;

        template <typename TValue>
        void value(const TValue &value);
        void strValue(const com::String value) { this->_value = value; }
        /**
         * @return the raw string value
         */
        com::String strValue() const { return this->_value; }
        const com::String &name() const { return _name; }
        int position() const { return _position; }
        bool empty() const { return _value.empty(); }

    private:
        com::String _name;
        com::String _value;
        int _position = UndefinedPosition;
    };
    ///////////////////////////////////////////////////////////////////////////

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
    TValue Parameter::value() const
    {
        if (_value.empty())
        {
            throw Exception(com::String("missing value for: " + name()));
        }
        return _convertStrValue<TValue>(_value);
    }

    template <typename TValue>
    void Parameter::value(const TValue &value)
    {
        com::StringStream ss;
        ss << value;
        this->_value = ss.str();
    }
}
