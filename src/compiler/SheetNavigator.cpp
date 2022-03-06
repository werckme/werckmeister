#include "SheetNavigator.h"
#include <stdlib.h>
#include <documentModel/objects/Voice.h>
#include <algorithm>
#include <unordered_map>
#include <com/common.hpp>
#include <compiler/metaCommands.h>
#include <com/config.hpp>
#include <com/werckmeister.hpp>
#include <compiler/commands/ACommand.h>
#include <compiler/argumentNames.h>
#include <com/IHasParameter.h>
#include <sstream>
#include "error.hpp"
#include <list>

namespace
{
    const com::String RepeatBegin("__repeat_begin_");
    const com::String RepeatEnd("__repeat_end_");
    const com::String RepeatBeginAndEnd("__repeat_begin_and_end_");
    struct Jump
    {
        com::String to;
        int numVisited = 0;
        int numVisitedTotal = 0;
        int numPerformed = 0;
        int numIgnore = 0;
        int numPerform = 0;
    };
    typedef std::unordered_map<size_t, Jump> Jumps;
    typedef std::unordered_map<com::String, size_t> Marks;
    typedef unsigned int SourcePosition;
    typedef std::unordered_map<SourcePosition, int> CueEventCountMap;
    void registerMark(size_t eventContainerIndex, const documentModel::Event &event, Marks &marks)
    {
        auto markCommand = com::getWerckmeister().solve<compiler::ACommand>(event.stringValue);
        markCommand->setArguments(event.metaArgs);
        com::IHasParameter::ParametersByNames &parameters = markCommand->getParameters();
        com::String name = parameters[argumentNames.Mark.Name].value<com::String>();
        auto it = marks.find(name);
        if (it == marks.end())
        {
            marks.insert(std::make_pair(name, eventContainerIndex));
        }
        else if (it->second != eventContainerIndex)
        {
            std::stringstream ss;
            ss << "marker duplicate with \"" << name << "\"";
            compiler::Exception exception(ss.str());
            exception << compiler::ex_sheet_source_info(event);
            throw exception;
        }
    }
    Jump &getJump(size_t eventContainerIndex, const documentModel::Event &event, Jumps &jumps)
    {
        auto it = jumps.find(eventContainerIndex);
        if (it != jumps.end())
        {
            return it->second;
        }
        auto jumpCommand = com::getWerckmeister().solve<compiler::ACommand>(event.stringValue);
        jumpCommand->setArguments(event.metaArgs);
        com::IHasParameter::ParametersByNames &parameters = jumpCommand->getParameters();
        Jump jump;
        jump.to = parameters[argumentNames.Jump.To].value<com::String>();
        int repeatValue = parameters[argumentNames.Jump.Repeat].value<int>();
        if (repeatValue + 1 > com::SheetNavigationMaxJumps)
        {
            std::stringstream ss;
            ss << "max repeat size exceeded = " << com::SheetNavigationMaxJumps << " repeats";
            compiler::Exception exception(ss.str());
            exception << compiler::ex_sheet_source_info(event);
            throw exception;
        }
        jump.numPerform = repeatValue + 1;
        jump.numIgnore = parameters[argumentNames.Jump.Ignore].value<int>();
        auto result = jumps.emplace(std::make_pair(eventContainerIndex, jump));
        return result.first->second;
    }

    std::string createInternalMarkerName(int id, int voltaSequenceNr = 0)
    {
        std::stringstream ss;
        ss << "__wm_" << id << "__";
        if (voltaSequenceNr > 0)
        {
            ss << "vg" << voltaSequenceNr;
        }
        return ss.str();
    }

    documentModel::Event createMarkerEvent(const com::String &id)
    {
        documentModel::Event event;
        event.stringValue = SHEET_META__MARK;
        event.type = documentModel::Event::Meta;
        documentModel::Argument arg;
        arg.name = argumentNames.Mark.Name;
        arg.value = id;
        event.metaArgs.emplace_back(arg);
        return event;
    }

    documentModel::Event createJumpEvent(const com::String &id, int repeat = 0, int ignore = 0)
    {
        documentModel::Event event;
        event.type = documentModel::Event::Meta;
        event.stringValue = SHEET_META__JUMP;
        {
            documentModel::Argument arg;
            arg.name = argumentNames.Jump.To;
            arg.value = id;
            event.metaArgs.emplace_back(arg);
        }
        {
            documentModel::Argument arg;
            arg.name = argumentNames.Jump.Ignore;
            arg.value = std::to_string(ignore);
            event.metaArgs.emplace_back(arg);
        }
        {
            documentModel::Argument arg;
            arg.name = argumentNames.Jump.Repeat;
            arg.value = std::to_string(repeat);
            event.metaArgs.emplace_back(arg);
        }
        return event;
    }

    documentModel::Event createVoltaJumpEvent(int srcVoltaNr, int voltaGrp)
    {
        documentModel::Event event = createJumpEvent(createInternalMarkerName(srcVoltaNr, voltaGrp), 0, srcVoltaNr - 1);
        return event;
    }

    int getVoltaNr(const documentModel::Event &event)
    {
        if (event.type != documentModel::Event::EOB)
        {
            return -1;
        }
        if (event.tags.empty())
        {
            return -1;
        }
        for (const auto &tag : event.tags)
        {
            if (tag.empty())
            {
                continue;
            }
            int voltaNr = atoi(tag.c_str());
            return voltaNr;
        }
        return -1;
    }

    void renameIfCueReappears(CueEventCountMap &cueEventCountMap, documentModel::Event& event)
    {
        bool isCueEvent = event.type == documentModel::Event::Meta && event.stringValue == SHEET_META__ADD_CUE;
        if (!isCueEvent) 
        {
            return;
        }
        auto mapIt = cueEventCountMap.find(event.sourcePositionBegin);
        if (mapIt == cueEventCountMap.end()) 
        {
            mapIt = cueEventCountMap.insert(std::make_pair(event.sourcePositionBegin, 1)).first;
        }
        int numAppearances = mapIt->second;
        ++mapIt->second;
        if (numAppearances == 1) 
        {
            return;
        }
        event.metaArgs.front().value = event.metaArgs.front().value + std::to_string(numAppearances);
    }
}

namespace compiler
{
    void SheetNavigator::processNavigation(documentModel::Voice &voice)
    {
        createRepeatJumps(voice);
        processJumps(voice);
    
    }

    void SheetNavigator::processJumps(documentModel::Voice &voice)
    {
        if (voice.events.empty())
        {
            return;
        }
        CueEventCountMap _cueEventCountMap;
        Jumps jumps;
        Marks marks;
        documentModel::Voice::Events &src = voice.events;
        std::list<documentModel::Event> dst;
        size_t length = src.size();
        // register marks
        for (size_t idx = 0; idx < length; ++idx)
        {
            const auto &event = src.at(idx);
            if (event.type != documentModel::Event::Meta || event.stringValue != SHEET_META__MARK)
            {
                continue;
            }
            registerMark(idx, event, marks);
        }
        // execute jumps
        for (size_t idx = 0; idx < length; ++idx)
        {
            const auto &event = src.at(idx);
            if (event.type != documentModel::Event::Meta || event.stringValue != SHEET_META__JUMP)
            {
                auto copy = event;
                renameIfCueReappears(_cueEventCountMap, copy);
                dst.emplace_back(copy);
                continue;
            }
            auto &jump = getJump(idx, event, jumps);
            auto markIt = marks.find(jump.to);
            if (markIt == marks.end())
            {
                std::stringstream ss;
                ss << "marker not found: \"" << jump.to << "\"";
                compiler::Exception exception(ss.str());
                exception << compiler::ex_sheet_source_info(event);
                throw exception;
            }
            bool jumpForward = markIt->second > idx;
            ++jump.numVisited;
            ++jump.numVisitedTotal;
            if (jump.numPerformed >= jump.numPerform)
            {                          // the jump has been perfomed
                jump.numPerformed = 0; // reset the counter for revisiting
                jump.numVisited = 0;
                continue;
            }
            if (jump.numVisited <= jump.numIgnore)
            {
                continue;
            }
            if ((jump.numVisitedTotal) > com::SheetNavigationMaxJumps)
            {
                std::stringstream ss;
                ss << "max jump size exceeded = " << com::SheetNavigationMaxJumps << " jumps";
                compiler::Exception exception(ss.str());
                exception << compiler::ex_sheet_source_info(event);
                throw exception;
            }
            if (jumpForward)
            {
                // in case of a forwad jump, reset counter imediately
                jump.numPerformed = 0;
                jump.numVisited = 0;
            }
            else
            {
                ++jump.numPerformed;
            }
            idx = markIt->second;
        }
        documentModel::Voice::Events copy(dst.begin(), dst.end());
        copy.swap(src);
    }

    void SheetNavigator::createRepeatJumps(documentModel::Voice &voice)
    {
        if (voice.events.empty())
        {
            return;
        }
        documentModel::Voice::Events &src = voice.events;
        std::list<documentModel::Event> dst;
        size_t length = src.size();
        int markCounter = 0;
        auto markAtBegin = createMarkerEvent(createInternalMarkerName(markCounter++));
        dst.emplace_front(markAtBegin);
        int voltaSequenceNr = 1;
        int lastVoltaNr = 0;
        auto voltaSeqBegin = dst.end();
        for (size_t idx = 0; idx < length; ++idx)
        {
            const auto &event = src.at(idx);
            dst.push_back(event);
            if (event.type != documentModel::Event::EOB)
            {
                continue;
            }
            int voltaNr = getVoltaNr(event);
            if (voltaNr != -1)
            { // we have a volta
                if (lastVoltaNr > 1 && voltaNr == 1)
                { // a new volta sequence begins
                    ++voltaSequenceNr;
                    lastVoltaNr = 0;
                }
                if (voltaNr == 1)
                { // remember the position of the first volta
                    // add a dummy event be one behind the EOB
                    dst.emplace_back(createMarkerEvent(createInternalMarkerName(0, voltaSequenceNr)));
                    voltaSeqBegin = --dst.end();
                }
                if (voltaNr - lastVoltaNr != 1)
                {
                    std::stringstream ss;
                    ss << "volta sequence is out of order with value '" << voltaNr << "'";
                    compiler::Exception exception(ss.str());
                    exception << compiler::ex_sheet_source_info(event);
                    throw exception;
                }
                if (voltaNr > 1)
                { // peform a jump from the first volta mark to the current location
                    dst.emplace_back(createMarkerEvent(createInternalMarkerName(voltaNr, voltaSequenceNr)));
                    auto voltaJump = createVoltaJumpEvent(voltaNr, voltaSequenceNr);
                    voltaSeqBegin = dst.insert(voltaSeqBegin, voltaJump);
                }
                lastVoltaNr = voltaNr;
            }
            // handle the repeats
            if (event.stringValue == RepeatBegin)
            {
                dst.emplace_back(createMarkerEvent(createInternalMarkerName(markCounter++)));
            }
            if (event.stringValue == RepeatEnd)
            {
                dst.emplace_back(createJumpEvent(createInternalMarkerName(markCounter - 1)));
            }
            if (event.stringValue == RepeatBeginAndEnd)
            {
                dst.emplace_back(createJumpEvent(createInternalMarkerName(markCounter - 1)));
                dst.emplace_back(createMarkerEvent(createInternalMarkerName(markCounter++)));
            }
        }
        documentModel::Voice::Events copy(dst.begin(), dst.end());
        copy.swap(src);
    }
}