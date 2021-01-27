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
    struct Jump {
        fm::String to;
        int numVisited = 0;
        int numVisitedTotal = 0;
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

    std::string createInternalMarkerName(int id, int voltaSequenceNr = 0)
    {
        std::stringstream ss;
        ss << "__wm_" << id << "__";
        if (voltaSequenceNr > 0) {
            ss << "vg" << voltaSequenceNr;
        }
        return ss.str();
    }

    sheet::Event createMarkerEvent(const fm::String& id)
    {
        sheet::Event event;
        event.stringValue = SHEET_META__MARK;
        event.type = sheet::Event::Meta;
        sheet::Argument arg;
        arg.name = argumentNames.Mark.Name;
        arg.value = id;
        event.metaArgs.emplace_back(arg);
        return event;
    }

    sheet::Event createJumpEvent(const fm::String &id, int repeat = 0, int ignore = 0)
    {
        sheet::Event event;
        event.type = sheet::Event::Meta;
        event.stringValue = SHEET_META__JUMP;
        {
            sheet::Argument arg;
            arg.name = argumentNames.Jump.To;
            arg.value = id;
            event.metaArgs.emplace_back(arg);
        }
        {
            sheet::Argument arg;
            arg.name = argumentNames.Jump.Ignore;
            arg.value = std::to_string(ignore);
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

    sheet::Event createVoltaJumpEvent(int srcVoltaNr, int voltaGrp)
    {
        sheet::Event event = createJumpEvent(createInternalMarkerName(srcVoltaNr, voltaGrp), 0, 1);
        return event;
    }

    int getVoltaNr(const sheet::Event &event)
    {
        if (event.type != sheet::Event::EOB) {
            return -1;
        }
        if (event.tags.empty()) {
            return -1;
        }
        for (const auto& tag : event.tags)
        {
            if (tag.empty()) {
                continue;
            }
            int voltaNr = atoi(tag.c_str());
            return voltaNr;
        }
        return -1;
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
                if (jump.numPerformed >= jump.numPerform) { // the jump has been perfomed
                    jump.numPerformed = 0; // reset the counter for revisiting
                    jump.numVisited = 0;
                    continue;
                }
                if (jump.numVisited <= jump.numIgnore) {
                    continue;
                }
                if ((++jump.numVisitedTotal) > fm::SheetNavigationMaxJumps) {
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
            Voice::Events& src = voice.events;
            std::list<sheet::Event> dst;
            size_t length = src.size();
            int markCounter = 0;
            auto markAtBegin = createMarkerEvent(createInternalMarkerName(markCounter++));
            dst.emplace_front(markAtBegin);
            int voltaSequenceNr = 1;
            int lastVoltaNr = 0;
            auto voltaSeqBegin = dst.end();
            for (size_t idx = 0; idx < length; ++idx) {
                const auto& event = src.at(idx);
                dst.push_back(event);
                if (event.type != Event::EOB) {
                    continue;
                }
                int voltaNr = getVoltaNr(event);
                if (voltaNr != -1) { // we have a volta
                    if (lastVoltaNr > 1 && voltaNr == 1) {  // a new volta sequence begins
                        ++voltaSequenceNr;
                        lastVoltaNr = 0;
                    }
                    if (voltaNr == 1) { // remember the position of the first volta
                        voltaSeqBegin = --dst.end();
                    }
                    if (voltaNr - lastVoltaNr != 1) {
                        std::stringstream ss;
                        ss << "volta sequence is out of order with value '" << voltaNr << "'";
                        sheet::compiler::Exception exception(ss.str());
                        exception << sheet::compiler::ex_sheet_source_info(event);
                        throw exception;
                    }
                    if (voltaNr > 1) { // peform a jump from the first volta mark to the current location
                        dst.emplace_back(createMarkerEvent(createInternalMarkerName(voltaNr, voltaSequenceNr)));
                        auto voltaJump = createVoltaJumpEvent(voltaNr, voltaSequenceNr);
                        voltaSeqBegin = dst.insert(++voltaSeqBegin, voltaJump);
                    }
                    lastVoltaNr = voltaNr;
                }
                // handle the repeats
                if (event.stringValue == RepeatBegin) {
                    dst.emplace_back(createMarkerEvent(createInternalMarkerName(markCounter++)));
                }
                if (event.stringValue == RepeatEnd) {
                    dst.emplace_back(createJumpEvent(createInternalMarkerName(markCounter - 1)));
                }
                if (event.stringValue == RepeatBeginAndEnd) {
                    dst.emplace_back(createJumpEvent(createInternalMarkerName(markCounter - 1)));
                    dst.emplace_back(createMarkerEvent(createInternalMarkerName(markCounter++)));
                }
            }
            Voice::Events copy(dst.begin(), dst.end());
            copy.swap(src);
        }
    }
}