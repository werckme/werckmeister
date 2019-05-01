#ifndef SHEET_TOOLS_H
#define SHEET_TOOLS_H

#include <fm/common.hpp>
#include <fm/exception.hpp>
#include <algorithm>
#include <vector>
#include <tuple>
#include <boost/algorithm/string.hpp>

namespace sheet {
    struct Event;

    

    template<class TString>
    struct NewLine {
        static const auto value() { return fm::String::value_type('\n'); }
    };

    template<>
    struct NewLine<fm::String> {
        static const auto value() { return fm::String::value_type('\n'); }
    };

    template<>
    struct NewLine<std::string> {
        static const auto value() { return std::string::value_type('\n'); }
    };

    template<>
    struct NewLine<fm::String::const_iterator> {
        static const auto value() { return fm::String::value_type('\n'); }
    };

    template<>
    struct NewLine<std::string::const_iterator> {
        static const auto value() { return std::string::value_type('\n'); }
    };

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

    template <typename TString>
    using LineAndPosition = std::tuple<TString, int>;


    /**
     * @return the number of lines until a position
     */ 
    template<typename TIterator>
    int getNumberOfLines(TIterator begin, TIterator end, int position)
    {
        if (begin + position >= end) {
            return -1;
        }
        return std::count_if(begin, begin + position, [](const auto &x) { return x == NewLine<TIterator>::value(); });
    }
    
    /**
     * @return the number of lines until a position
     */ 
    template<typename TString>
    int getNumberOfLines(const TString &str, int position)
    {
        return getNumberOfLines(str.begin(), str.end(), position);
    }


    /**
     * @return the line at a given position
     */ 
    template<typename TIterator, typename TString>
    LineAndPosition<TString> getLineAndPosition(TIterator begin, 
        TIterator end, int sourcePosition, bool trim = true)
    {
        if (sourcePosition >= (end - begin)) {
            return LineAndPosition<TString>(TString(), -1);
        }
        auto it = begin, 
             start = begin;

        // find begin of line
        it = begin + sourcePosition;
        while(it > begin) {
            if (*it == NewLine<TString>::value()) {
                start = it;
                break;
            }
			--it;
        }
        // find end of line
        it = begin + sourcePosition;
        while(it < end) {
            if (*it == NewLine<TString>::value()) {
                end = it;
                break;
            }
			it++;
        }
        int position = sourcePosition - std::distance(begin, start);
        auto line = TString(start, end);
        if (trim) {
            int diff = line.length();
            boost::trim(line);
            diff = diff - line.length();
            position = std::max(-1, position - diff);
        }
        return LineAndPosition<TString>(line, position);
    }

    /**
     * @return the line at a given position
     */ 
    template<typename TString>
    inline LineAndPosition<TString> getLineAndPosition(const TString &source, int sourcePosition, bool trim = true)
    {
        return getLineAndPosition<typename TString::const_iterator, TString>(source.begin(), source.end(), sourcePosition, trim);
    }

    /**
     *  replaces a comment sequence with an spaces.
     */
    template<typename TIterator>
    void removeComments(TIterator begin, TIterator end)
    {
        auto it = begin;
        bool clearing = false;
        while (it != end) {
            if (*it == NewLine<TIterator>::value()) {
                clearing = false;
            }
            if (*it == '-') {
                if ((it + 1) != end && *(it + 1) == '-') {
                    clearing = true;
                }
            }
            if (clearing) {
                *it = ' ';
            }
            ++it;
        }
    }

    fm::String pitchToString(fm::Pitch pitch);

    typedef std::tuple<int, int> RowAndColumn;
    extern RowAndColumn InvalidRowAndColumn;

    template<class TIterator>
    std::vector<RowAndColumn> getRowsAndColumns(TIterator begin, TIterator end, const std::vector<int> &_positions)
    {
        std::vector<RowAndColumn> result;
        if (begin == end || _positions.empty()) {
            return result;
        }
        auto positions = _positions;
        std::sort(positions.begin(), positions.end());
        auto it = begin;
        int row = 0;
        int column = 0;
        int position = 0;
        auto currentPosition = positions.begin();
        for (;it != end; ++it) {
            if (*it == NewLine<TIterator>::value()) {
                ++row;
                column = 0;
                ++position;
                continue;
            }
            if (*currentPosition == position) {
                result.push_back(RowAndColumn(row, column));
                ++currentPosition;
                if (currentPosition == positions.end()) {
                    break;
                }
            }
            ++column;
            ++position;
        }
        return result;
    }

    template<class TIterator>
    RowAndColumn getRowAndColumn(TIterator begin, TIterator end, int position)
    {
        auto rowsAndColumns = getRowsAndColumns(begin, end, {position} );
        if (rowsAndColumns.empty()) {
            return InvalidRowAndColumn;
        }
        return *(rowsAndColumns.begin());
    }
}

#endif