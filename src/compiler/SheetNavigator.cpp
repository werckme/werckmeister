#include "SheetNavigator.h"
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

namespace {
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
        jump.numPerform = parameters[argumentNames.Jump.Repeat].value<int>() + 1;
        jump.numIgnore = parameters[argumentNames.Jump.Ignore].value<int>();
        auto result = jumps.emplace(std::make_pair(eventContainerIndex, jump));
        return result.first->second;
    }
}

namespace sheet {
    namespace compiler {
        void SheetNavigator::processNavigation(Voice& voice)
        {
            if (voice.events.empty()) {
                return;
            }
            Jumps jumps;
            Marks marks;
            Voice::Events& src = voice.events;
            Voice::Events dst;
            dst.reserve(src.size());
            size_t length = src.size();
            // register marks
            for (size_t idx = 0; idx < length; ++idx) {
                const auto& event = src.at(idx);
                if (event.type != Event::Meta || event.stringValue != SHEET_META__MARK) {
                    continue;
                }
                registerMark(idx, event, marks);
            }
            // process jumps
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
                if (jump.numVisited >= fm::SheetNavigationMaxJumps) {
                    std::stringstream ss;
                    ss << "max jump size exceeded: " << fm::SheetNavigationMaxJumps;
                    sheet::compiler::Exception exception(ss.str());
                    exception << sheet::compiler::ex_sheet_source_info(event);
                    throw exception;
                }
                ++jump.numPerformed;
                idx = markIt->second;
            }
            dst.swap(src);
        }
    }
}