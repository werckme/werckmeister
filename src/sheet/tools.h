#ifndef SHEET_TOOLS_H
#define SHEET_TOOLS_H

#include <fm/common.hpp>
#include <fm/exception.hpp>
#include <algorithm>
#include <vector>

namespace sheet {
    class Event;
    namespace toolsimpl {
        const std::vector<fm::String> & getMetaArgs(const Event &metaEvent);
        const fm::String & getMetaCommand(const Event &metaEvent);
    }
    namespace {
        struct MissingArgument {};
        template<typename TArg, typename TArgs>
        TArg __getArgument(const TArgs &args, int idx, TArg *defaultValue) 
        {
            if (idx >= (int)args.size()) {
                if (defaultValue) {
                    return *defaultValue;
                }
                throw MissingArgument();
            }
            TArg result;
            fm::StringStream ss;
            ss << args[idx];
            ss >> result;
            return result;
        }		
    }
    
    template<typename TArg>
    TArg getArgument(const Event &metaEvent, int idx, TArg *defaultValue = nullptr) 
    {
        try {
            return __getArgument<TArg>(toolsimpl::getMetaArgs(metaEvent), idx, defaultValue);
        } catch(const MissingArgument&) {
            FM_THROW(fm::Exception, "missing argument for '" + fm::to_string( toolsimpl::getMetaCommand(metaEvent) ) + "'");
        }
    }

    template<typename TArg, typename TArgs>
    TArg getArgument(const TArgs &args, int idx, TArg *defaultValue = nullptr) 
    {
        try {
            return __getArgument<TArg, TArgs>(args, idx, defaultValue);
        } catch(const MissingArgument&) {
            FM_THROW(fm::Exception, "missing meta argumnet");
        }
    }

    template<class TMetaInfoContainer>
    auto getMetaValuesBy(const fm::String &name, const TMetaInfoContainer& container, bool required = false)
    {
        auto it = std::find_if(container.begin(), 
                               container.end(), 
                               [name](const auto &x) { return x.name == name; });
        if (it == container.end()) {
            if (required) {
                FM_THROW(fm::Exception, "no value for meta key: " + fm::to_string(name));
            }
            return typename TMetaInfoContainer::value_type::Args();
        }
        return it->args;
    } 
    
    template<class TMetaInfoContainer>
    fm::String getFirstMetaValueBy(const fm::String &name, const TMetaInfoContainer& container, bool required = false)
    {
        auto values = getMetaValuesBy(name, container, required);
        if (values.empty()) {
            // no required check needed here (getMetaValuesBy throws already)
            return fm::String();
        }
        return *values.begin();
    }

    template <class TContainer>
    void append(TContainer &dst, const TContainer &toAppend)
    {
        if (toAppend.empty()) {
            return;
        }
        dst.insert(dst.end(), toAppend.begin(), toAppend.end());
    }

    /**
     * returns a value from an argument list such as:
     * key1 value1 key2 value2 ...
     * @return std::pair(found, theValue)
     **/
    template <typename TValue, class TContainer>
    std::pair<bool, TValue> getArgValueFor(const typename TContainer::value_type &key, const TContainer &container)
    {
        auto defaultValue = TValue();
        if (key.empty()) {
            return std::make_pair(false, defaultValue);
        }
        size_t idx = 0;
        for(const auto &x : container) {
            if (x == key) {
                if (idx+1 >= container.size()) {
                    return std::make_pair(false, defaultValue);
                }
                auto result = getArgument<TValue>(container, idx+1, &defaultValue);
                return std::make_pair(true, result);
            }
            ++idx;
        }
        return std::make_pair(false, defaultValue);
    }

    template <typename TEventContainer>
    bool hasAnyTimeConsumingEvents(const TEventContainer &events) 
    {
        return std::find_if(
                events.begin(), 
                events.end(), 
                [](const auto &x) { return x.isTimeConsuming(); }) != events.end();
    }

}

#endif