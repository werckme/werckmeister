#pragma once

#include <com/common.hpp>
#include <com/exception.hpp>
#include <algorithm>
#include <vector>
#include <tuple>
#include <boost/algorithm/string.hpp>
#include <documentModel/objects/ASheetObjectWithSourceInfo.h>
#include <documentModel/Argument.h>
#include <com/IHasParameter.h>
#include <ostream>

namespace documentModel
{
    struct Event;
    struct Track;
    class Document;
}

namespace com
{

    enum
    {
        NO_ARG_POSITION = -1
    };

    void argumentsToParameters(const std::vector<documentModel::Argument> arguments, IHasParameter::ParametersByNames &outParameters);

    template <class TString>
    struct NewLine
    {
        static const auto value() { return com::String::value_type('\n'); }
    };

    template <>
    struct NewLine<com::String>
    {
        static const auto value() { return com::String::value_type('\n'); }
    };

    template <>
    struct NewLine<com::String::const_iterator>
    {
        static const auto value() { return com::String::value_type('\n'); }
    };

    namespace toolsimpl
    {
        const std::vector<documentModel::Argument> &getMetaArgs(const documentModel::Event &metaEvent);
        const com::String &getMetaCommand(const documentModel::Event &metaEvent);
    }

    namespace
    {
        struct MissingArgument
        {
        };

        template <typename TArgs>
        const documentModel::Argument *__getArgument(const TArgs &args, int idx)
        {
            if (idx >= (int)args.size())
            {
                return nullptr;
            }
            return &args[idx];
        }

        template <typename TValue, typename TArgs>
        TValue __getArgumentValue(const TArgs &args, int idx, const TValue *defaultValue)
        {
            if (idx == NO_ARG_POSITION)
            {
                if (defaultValue)
                {
                    return *defaultValue;
                }
                throw MissingArgument();
            }
            const documentModel::Argument *argument = __getArgument(args, idx);
            if (!argument)
            {
                if (defaultValue)
                {
                    return *defaultValue;
                }
                throw MissingArgument();
            }
            return argument->parseValue<TValue>();
        }

    }

    template <typename TArg>
    TArg getArgumentValue(const documentModel::Event &metaEvent, int idx, TArg *defaultValue = nullptr)
    {
        try
        {
            return __getArgumentValue<TArg>(toolsimpl::getMetaArgs(metaEvent), idx, defaultValue);
        }
        catch (const MissingArgument &)
        {
            FM_THROW(com::Exception, "missing argument for '" + toolsimpl::getMetaCommand(metaEvent) + "'");
        }
    }

    template <typename TArg, typename TArgs>
    TArg getArgumentValue(const TArgs &args, int idx, TArg *defaultValue = nullptr)
    {
        try
        {
            return __getArgumentValue<TArg, TArgs>(args, idx, defaultValue);
        }
        catch (const MissingArgument &)
        {
            FM_THROW(com::Exception, "missing meta argument");
        }
    }

    template <typename TArgs>
    documentModel::Argument getArgument(const TArgs &args, int idx)
    {
        auto result = __getArgument<TArgs>(args, idx);
        if (!result)
        {
            FM_THROW(com::Exception, "missing meta argument");
        }
        return *result;
    }

    template <class TMetaInfoContainer>
    auto getMetaArgumentsForKey(const com::String &name, const TMetaInfoContainer &container, bool required = false)
    {
        auto it = std::find_if(container.begin(),
                               container.end(),
                               [name](const auto &x)
                               { return x.name == name; });
        if (it == container.end())
        {
            if (required)
            {
                FM_THROW(com::Exception, "no value for meta key: " + name);
            }
            return typename TMetaInfoContainer::value_type::Args();
        }
        return it->args;
    }

    template <class TMetaInfoContainer>
    documentModel::Argument getFirstMetaArgumentForKey(const com::String &name, const TMetaInfoContainer &container, bool required = false)
    {
        auto values = getMetaArgumentsForKey(name, container, required);
        if (values.empty())
        {
            // no required check needed here (getMetaArgumentsForKey throws already)
            return documentModel::Argument();
        }
        return *values.begin();
    }

    template <class TContainer>
    void append(TContainer &dst, const TContainer &toAppend)
    {
        if (toAppend.empty())
        {
            return;
        }
        dst.insert(dst.end(), toAppend.begin(), toAppend.end());
    }

    template <class TContainer>
    void insertRange(TContainer &dst, const TContainer &toAppend)
    {
        if (toAppend.empty())
        {
            return;
        }
        dst.insert(toAppend.begin(), toAppend.end());
    }

    /**
     * returns a value from an argument list such as:
     * key1 value1 key2 value2 ...
     * @return std::pair(found, theValue)
     **/
    DEPRECATED
    template <typename TValue, class TContainer>
    std::pair<bool, TValue> getArgValueFor(const com::String &key, const TContainer &container)
    {
        auto defaultValue = TValue();
        if (key.empty())
        {
            return std::make_pair(false, defaultValue);
        }
        size_t idx = 0;
        for (const auto &x : container)
        {
            if (x.value == key)
            {
                if (idx + 1 >= container.size())
                {
                    return std::make_pair(false, defaultValue);
                }
                auto result = getArgumentValue<TValue>(container, idx + 1, &defaultValue);
                return std::make_pair(true, result);
            }
            ++idx;
        }
        return std::make_pair(false, defaultValue);
    }

    /**
     * returns {
     *      keyword1: [value1, value2]
     *      keyword2: [value3, value4] 
     * }
     * for: "keyword1 value1 value2 keyword2 value3 value4"
     * 
     * {   
     *      "": [[value0]]
     *      keyword1: [[value1, value2], [value3, value4]]
     *      keyword2: [[value4, value5]]
     * }
     * for: "value0 keyword1 value1 value2 keyword 1 value2 value3 keyword2 value4 value5"
     **/
    template <class TArgContainer, class TKeywordContainer>
    std::multimap<com::String, std::vector<documentModel::Argument>>
    mapArgumentsByKeywords(const TArgContainer &args, const TKeywordContainer &keywords)
    {
        std::multimap<com::String, std::vector<documentModel::Argument>> result;
        auto it = args.begin();
        auto end = args.end();
        com::String keyword = "";
        bool defaultKeywordCreated = false;
        for (; it != end; ++it)
        {
            bool isKeyword = std::find(keywords.begin(), keywords.end(), it->value) != keywords.end();
            if (isKeyword)
            {
                keyword = it->value;
                result.emplace(std::make_pair(keyword, TArgContainer()));
                continue;
            }
            if (keyword == "" && !defaultKeywordCreated)
            {
                result.emplace(std::make_pair(keyword, TArgContainer()));
                defaultKeywordCreated = true;
            }
            auto mapIt = result.upper_bound(keyword);
            --mapIt;
            mapIt->second.push_back(*it);
        }
        return result;
    }

    /**
     * returns a value from an argument list such as:
     * key1 value1 key2 value2 ...
     * @return theValue or default value
     **/
    DEPRECATED
    template <typename TValue, class TContainer>
    TValue getArgValueFor(const com::String &key, const TContainer &container, const TValue &defaultValue)
    {
        auto result = getArgValueFor<TValue>(key, container);
        if (!result.first)
        {
            return defaultValue;
        }
        return result.second;
    }

    template <typename TEventContainer>
    bool hasAnyTimeConsumingEvents(const TEventContainer &events)
    {
        return std::find_if(
                   events.begin(),
                   events.end(),
                   [](const auto &x)
                   { return x.isTimeConsuming(); }) != events.end();
    }

    template <typename TString>
    using LineAndPosition = std::tuple<TString, int>;

    /**
     * @return the number of lines until a position
     */
    template <typename TIterator>
    int getNumberOfLines(TIterator begin, TIterator end, int position)
    {
        if (begin + position >= end)
        {
            return -1;
        }
        return std::count_if(begin, begin + position, [](const auto &x)
                             { return x == NewLine<TIterator>::value(); });
    }

    /**
     * @return the number of lines until a position
     */
    template <typename TString>
    int getNumberOfLines(const TString &str, int position)
    {
        return getNumberOfLines(str.begin(), str.end(), position);
    }

    /**
     * @return the line at a given position
     */
    template <typename TIterator, typename TString>
    LineAndPosition<TString> getLineAndPosition(TIterator begin,
                                                TIterator end, int sourcePosition, bool trim = true)
    {
        if (sourcePosition >= (end - begin))
        {
            return LineAndPosition<TString>(TString(), -1);
        }
        auto it = begin,
             start = begin;

        // find begin of line
        it = begin + sourcePosition;
        while (it > begin)
        {
            if (*it == NewLine<TString>::value())
            {
                start = it;
                break;
            }
            --it;
        }
        // find end of line
        it = begin + sourcePosition;
        while (it < end)
        {
            if (*it == NewLine<TString>::value())
            {
                end = it;
                break;
            }
            it++;
        }
        int position = sourcePosition - std::distance(begin, start);
        auto line = TString(start, end);
        if (trim)
        {
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
    template <typename TString>
    inline LineAndPosition<TString> getLineAndPosition(const TString &source, int sourcePosition, bool trim = true)
    {
        return getLineAndPosition<typename TString::const_iterator, TString>(source.begin(), source.end(), sourcePosition, trim);
    }

    /**
     *  replaces a comment sequence with spaces.
     */
    template <typename TIterator>
    void removeComments(TIterator begin, TIterator end)
    {
        auto it = begin;
        bool clearing = false;
        bool isMultiline = false;
        while (it != end)
        {
            if (!isMultiline && *it == NewLine<TIterator>::value())
            {
                clearing = false;
            }
            if (!clearing && *it == '/') {
                if ((it + 1) != end && *(it + 1) == '*')
                {
                    clearing = true;
                    isMultiline = true;
                }
            }
            if (isMultiline && *it == '*') {
                if ((it + 1) != end && *(it + 1) == '/')
                {
                    clearing = false;
                    isMultiline = false;
                    *it = ' ';
                    *(it + 1) = ' ';
                }
            }
            if (!isMultiline && *it == '-')
            {
                if ((it + 1) != end && *(it + 1) == '-')
                {
                    clearing = true;
                }
            }
            if (clearing)
            {
                *it = ' ';
            }
            ++it;
        }
    }

    com::String pitchToString(com::Pitch pitch);

    typedef std::tuple<int, int> RowAndColumn;
    extern RowAndColumn InvalidRowAndColumn;

    template <class TIterator>
    std::vector<RowAndColumn> getRowsAndColumns(TIterator begin, TIterator end, const std::vector<int> &_positions)
    {
        std::vector<RowAndColumn> result;
        if (begin == end || _positions.empty())
        {
            return result;
        }
        auto positions = _positions;
        std::sort(positions.begin(), positions.end());
        auto it = begin;
        int row = 0;
        int column = 0;
        int position = 0;
        auto currentPosition = positions.begin();
        for (; it != end; ++it)
        {
            if (*it == NewLine<TIterator>::value())
            {
                ++row;
                column = 0;
                ++position;
                continue;
            }
            if (*currentPosition == position)
            {
                result.push_back(RowAndColumn(row, column));
                ++currentPosition;
                if (currentPosition == positions.end())
                {
                    break;
                }
            }
            ++column;
            ++position;
        }
        return result;
    }

    template <class TIterator>
    RowAndColumn getRowAndColumn(TIterator begin, TIterator end, int position)
    {
        auto rowsAndColumns = getRowsAndColumns(begin, end, {position});
        if (rowsAndColumns.empty())
        {
            return InvalidRowAndColumn;
        }
        return *(rowsAndColumns.begin());
    }
    std::ostream &documentMessageWhere(std::ostream &ss, const std::string filename, int line = -1);
    std::ostream &documentMessageWhat(std::ostream &ss, const std::string &what);
    std::ostream &documentMessage(std::ostream &ss,
                                  const std::shared_ptr<documentModel::Document>,
                                  documentModel::ASheetObjectWithSourceInfo::SourceId,
                                  unsigned int sourcePosition,
                                  const std::string &message);
    
    bool isNoteEventTrack(const documentModel::Track&);
    bool isAccompEventTrack(const documentModel::Track&);
    bool isTemplateTrack(const documentModel::Track&);

}
