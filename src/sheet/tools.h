#ifndef SHEET_TOOLS_H
#define SHEET_TOOLS_H

#include <fm/common.hpp>
#include <fm/exception.hpp>
#include <algorithm>

namespace sheet {
    class Event;
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
            return __getArgument<TArg>(metaEvent.metaArgs, idx, defaultValue);
        } catch(const MissingArgument&) {
            FM_THROW(fm::Exception, "missing argument for '" + fm::to_string(metaEvent.metaCommand) + "'");
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

}

#endif