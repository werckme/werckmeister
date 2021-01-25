#include "SheetNavigator.h"
#include <stdlib.h>
#include <sheet/objects/Voice.h>
#include <algorithm>
#include <unordered_map>
#include <fm/common.hpp>
#include <compiler/metaCommands.h>
#include <fm/config.hpp>
#include <fm/werckmeister.hpp>
#include <compiler/commands/ACommand.h>
#include <compiler/argumentNames.h>
#include <fm/IHasParameter.h>
#include <sstream>
#include "error.hpp"
#include <list>

namespace {
    const fm::String RepeatBegin("__repeat_begin_");
    const fm::String RepeatEnd("__repeat_end_");
    const fm::String RepeatBeginAndEnd("__repeat_begin_and_end_");
    const char RepeatChar = 'x';
    const char JumpMarkChar = '^';
    struct Jump {
        fm::String to;
        int numVisited = 0;
        int numPerformed = 0;
        int numIgnore = 0;
        int numPerform = 0;
    };
    typedef std::unordered_map<size_t, Jump> Jumps;
    typedef std::unordered_map<fm::String, size_t> Marks;
    void registerMark(size_t eventContainerIndex, const sheet::Event& event, Marks &marks)
    {
        auto markCommand = fm::getWerckmeister().solve<sheet::compiler::ACommand>(event.stringValue);
        markCommand->setArguments(event.metaArgs);
        fm::IHasParameter::ParametersByNames &parameters = markCommand->getParameters();
        fm::String name = parameters[argumentNames.Mark.Name].value<fm::String>();
        auto it = marks.find(name);
        if (it == marks.end()) {
            marks.insert(std::make_pair(name, eventContainerIndex));
        }
        else if (it->second != eventContainerIndex) {
            std::stringstream ss;
            ss << "marker duplicate with \"" << name << "\"";
            sheet::compiler::Exception exception(ss.str());
            exception << sheet::compiler::ex_sheet_source_info(event);
            throw exception;
        }
    }
    Jump & getJump(size_t eventContainerIndex, const sheet::Event& event, Jumps& jumps)
    {
        auto it = jumps.find(eventContainerIndex);
        if (it != jumps.end()) {
            return it->second;
        }
        auto jumpCommand = fm::getWerckmeister().solve<sheet::compiler::ACommand>(event.stringValue);
        jumpCommand->setArguments(event.metaArgs);
        fm::IHasParameter::ParametersByNames& parameters = jumpCommand->getParameters();
        Jump jump;
        jump.to = parameters[argumentNames.Jump.To].value<fm::String>();
        int repeatValue = parameters[argumentNames.Jump.Repeat].value<int>();
        if (repeatValue+1 > fm::SheetNavigationMaxJumps) {
            std::stringstream ss;
            ss << "max repeat size exceeded = " << fm::SheetNavigationMaxJumps << " repeats";
            sheet::compiler::Exception exception(ss.str());
            exception << sheet::compiler::ex_sheet_source_info(event);
            throw exception;
        }
        jump.numPerform = repeatValue + 1;
        jump.numIgnore = parameters[argumentNames.Jump.Ignore].value<int>();
        auto result = jumps.emplace(std::make_pair(eventContainerIndex, jump));
        return result.first->second;
    }

    std::string createInternalMarkerName(int id)
    {
        std::stringstream ss;
        ss << "__wm_" << id << "__";
        return ss.str();
    }

    sheet::Event createMarkerEvent(int id) 
    {
        sheet::Event event;
        event.stringValue = SHEET_META__MARK;
        event.type = sheet::Event::Meta;
        sheet::Argument arg;
        arg.name = argumentNames.Mark.Name;
        arg.value = createInternalMarkerName(id);
        event.metaArgs.emplace_back(arg);
        return event;
    }

    sheet::Event createJumpEvent(int id, int repeat = 0)
    {
        sheet::Event event;
        event.type = sheet::Event::Meta;
        event.stringValue = SHEET_META__JUMP;
        {
            sheet::Argument arg;
            arg.name = argumentNames.Jump.To;
            arg.value = createInternalMarkerName(id);
            event.metaArgs.emplace_back(arg);
        }
        {
            sheet::Argument arg;
            arg.name = argumentNames.Jump.Ignore;
            arg.value = std::to_string(0);
            event.metaArgs.emplace_back(arg);
        }
        {
            sheet::Argument arg;
            arg.name = argumentNames.Jump.Repeat;
            arg.value = std::to_string(repeat);
            event.metaArgs.emplace_back(arg);
        }
        return event;
    }

    int getNumRepeats(const sheet::Event& eob) 
    {
        if (eob.type != sheet::Event::EOB) {
            return 0;
        }
        if (eob.tags.empty()) {
            return 0;
        }
        for (const auto& tag : eob.tags)
        {
            if (tag.length() < 2) {
                continue;
            }
            if (tag.at(0) != RepeatChar) {
                continue;
            }
            fm::String val(tag.begin() + 1, tag.end());
            int result = atoi(val.c_str());
            if (result < 2) {
                std::stringstream ss;
                ss << "repeat minimum is 'x2'" << std::endl;
                sheet::compiler::Exception exception(ss.str());
                exception << sheet::compiler::ex_sheet_source_info(eob);
                throw exception;
            }
            return result - 1;
        }
        return 0;
    }

    std::vector<int> getVoltaList(const sheet::Voice& voice)
    {
        std::vector<int> result;
        for (const auto& event : voice.events) {
            if (event.type != sheet::Event::EOB) {
                continue;
            }
            if (event.tags.empty()) {
                continue;
            }
            for (const auto& tag : event.tags)
            {
                if (tag.length() < 2) {
                    continue;
                }
                if (tag.at(0) != RepeatChar) {
                    continue;
                }
                fm::String val(tag.begin() + 1, tag.end());
                int markNr = atoi(val.c_str());
                result.push_back(markNr);
            }
        }
        return result;
    }
}

namespace sheet {
    namespace compiler {
        void SheetNavigator::processNavigation(Voice& voice)
        {
            processRepeats(voice);
            processJumps(voice);
        }
        void SheetNavigator::processJumps(Voice& voice)
        {
            if (voice.events.empty()) {
                return;
            }
            Jumps jumps;
            Marks marks;
            Voice::Events& src = voice.events;
            std::list<sheet::Event> dst;
            size_t length = src.size();
            // register marks
            for (size_t idx = 0; idx < length; ++idx) {
                const auto& event = src.at(idx);
                if (event.type != Event::Meta || event.stringValue != SHEET_META__MARK) {
                    continue;
                }
                registerMark(idx, event, marks);
            }
            // execute jumps
            for (size_t idx = 0; idx < length; ++idx) {
                const auto& event = src.at(idx);
                if (event.type != Event::Meta || event.stringValue != SHEET_META__JUMP) {
                    dst.push_back(event);
                    continue;
                }
                auto& jump = getJump(idx, event, jumps);
                auto markIt = marks.find(jump.to);
                if (markIt == marks.end()) {
                    std::stringstream ss;
                    ss << "marker not found: \"" << jump.to << "\"";
                    sheet::compiler::Exception exception(ss.str());
                    exception << sheet::compiler::ex_sheet_source_info(event);
                    throw exception;
                }
                ++jump.numVisited;
                if (jump.numPerformed >= jump.numPerform) {
                    continue;
                }
                if (jump.numVisited <= jump.numIgnore) {
                    continue;
                }
                if (jump.numVisited > fm::SheetNavigationMaxJumps) {
                    std::stringstream ss;
                    ss << "max jump size exceeded = " << fm::SheetNavigationMaxJumps << " jumps";
                    sheet::compiler::Exception exception(ss.str());
                    exception << sheet::compiler::ex_sheet_source_info(event);
                    throw exception;
                }
                ++jump.numPerformed;
                idx = markIt->second;
            }
            Voice::Events copy(dst.begin(), dst.end());
            copy.swap(src);
        }
        
        void SheetNavigator::processRepeats(Voice& voice)
        {
            if (voice.events.empty()) {
                return;
            }
            Jumps jumps;
            Marks marks;
            Voice::Events& src = voice.events;
            std::list<sheet::Event> dst;
            size_t length = src.size();
            int markCounter = 0;
            dst.emplace_front(createMarkerEvent(markCounter++));
            for (size_t idx = 0; idx < length; ++idx) {
                const auto& event = src.at(idx);
                dst.push_back(event);
                if (event.type != Event::EOB) {
                    continue;
                }
                if (event.stringValue == RepeatBegin) {
                    dst.emplace_back(createMarkerEvent(markCounter++));
                }
                if (event.stringValue == RepeatEnd) {
                    dst.emplace_back(createJumpEvent(markCounter - 1, getNumRepeats(event)));
                }
                if (event.stringValue == RepeatBeginAndEnd) {
                    dst.emplace_back(createJumpEvent(markCounter - 1, getNumRepeats(event)));
                    dst.emplace_back(createMarkerEvent(markCounter++));
                }
            }
            Voice::Events copy(dst.begin(), dst.end());
            copy.swap(src);
        }
    }
}