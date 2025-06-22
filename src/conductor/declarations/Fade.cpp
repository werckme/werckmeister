#include "Fade.h"
#include <regex>
#include <string>
#include <iostream> 
#include <compiler/commands/SetCC.h>
#include <boost/algorithm/string.hpp>
#include <charconv> 
#include "Cc.h"
#include <compiler/commands/FadeCC.h>
#include <com/tween.h>
#include <cassert>
#include <functional>
#include <com/midi.hpp>

namespace conductor
{

    namespace
    {
        double parseNumber(const com::String& str, const com::String& context)
        {
            try {
                return std::stod(str);
            } catch (...) {
                FM_THROW(compiler::Exception, "invalid double value '" + str + "' for " + context);
            }
        }

        void parseOption(Fade::FadeOptions& options, const com::String& strOptionExpr)
        {
            std::regex pattern("(\\w+)\\s*\\(\\s*([a-zA-Z0-9.-]+)\\s*\\)");
            std::smatch match;
            if (!std::regex_search(strOptionExpr, match, pattern))
            {
                FM_THROW(compiler::Exception, "invalid fade expression: " + strOptionExpr);
            }
            com::String strOption = match[1].str();
            com::String strValue = match[2].str();
            if (strOption == "type")
            {
                if (strValue == "bend")
                {
                    options.type = Fade::FadeOptions::TypeBend;
                    return;
                }
                const auto& contollerNameMap = Cc::getControllerNumberMap();
                auto ccIt = contollerNameMap.find(strValue);
                if (ccIt == contollerNameMap.end())
                {
                    FM_THROW(compiler::Exception, "invalid cc name: '" + strValue + "'. Please find here a complete list of supported cc names https://werckme.github.io/manual#cc");

                }
                options.type = ccIt->second;
                return;
            }
            if (strOption == "from")
            {
                options.from = (int)parseNumber(strValue, "from");
                return;
            }
            if (strOption == "to")
            {
                options.to = (int)parseNumber(strValue, "to");
                return;
            }
            if (strOption == "curve")
            {
                options.curve = strValue;
                return;
            }
            if (strOption == "offset")
            { 
            
                options.offset = (com::Ticks)parseNumber(strValue, "offset");
                return;
            }
            if (strOption == "duration")
            { 
                options.duration = (com::Ticks)parseNumber(strValue, "duration");
                if (options.duration <= 0)
                {
                    FM_THROW(compiler::Exception, "invalid duration value: " + strOption);
                }
                return;
            }
            FM_THROW(compiler::Exception, "unkown fade option: " + strOption);
        }
   
        template <class TTween>
        void _perform(com::midi::Track* track, com::Ticks startPos, int channel, const TTween &tween, com::Ticks duration, int valueType)
        {
            using namespace com;
            std::function<com::midi::Event(com::Ticks, double)> createEvent = [channel, valueType](com::Ticks pos, double val)
            {
                if (valueType == Fade::FadeOptions::TypeBend)
                {
                    return com::midi::Event::PitchBend(channel, pos, val/100.0);
                }
                auto ev = com::midi::Event::CCValue(channel, valueType, val);
                ev.absPosition(pos);
                return ev;
            };
            for (double t = 0; t <= duration; t += double(1.0_N128))
            {
                auto doubleValue = tween.calc(t);
                int value = std::max(0, std::min(127, int(doubleValue)));
                auto event = createEvent(t + startPos, value);
                track->events().add(event);
            }
        }
    }

    Fade::Fade(com::midi::MidiPtr midi) : midi(midi) 
    {
    }

    void Fade::parseFadeOptions(FadeOptions& options, const com::String& input)
    {
        std::vector<com::String> strOptions;
        if (input.empty())
        {
            return;
        }
        boost::split(strOptions, input, boost::is_any_of(","));
        for(const auto& strOption : strOptions)
        {
            parseOption(options, strOption);
        }
    }

    void Fade::perform(const Events &events) const
    {
        if (declaration.unit == documentModel::ConductionRule::Declaration::UnitPercent)
        {
            FM_THROW(compiler::Exception, "the unit precent isn't supported by fade");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationAdd)
        {
            FM_THROW(compiler::Exception, "the add operator isn't supported by fade");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationSubstract)
        {
            FM_THROW(compiler::Exception, "the substract operator isn't supported by fade");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpAdd)
        {
            FM_THROW(compiler::Exception, "the follow up add operator isn't supported by fade");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract)
        {
            FM_THROW(compiler::Exception, "the follow up substract operator isn't supported by fade");
        }
        std::string input = declaration.strValue();
        FadeOptions options;
        parseFadeOptions(options, input);
        auto track = events.midiEvent->parentTrack;
        assert(track != nullptr);
        com::Ticks duration = options.duration * com::PPQ;
        int maxValue = options.type == FadeOptions::TypeBend ? 100: 127;
        int channel = events.midiEvent->channel();
        com::Ticks startPos = events.midiEvent->absPosition();
        startPos += options.offset;
        if (startPos < 0)
        {
            startPos = 0;
        }
        if (options.from < 0 || options.from > maxValue)
        {
            FM_THROW(compiler::Exception, "fade value out of range max(" + std::to_string(maxValue)  +  "): " + std::to_string(options.from));
        }
        if (options.to < 0 || options.to > maxValue)
        {
            FM_THROW(compiler::Exception, "fade value out of range max(" + std::to_string(maxValue)  +  "): " + std::to_string(options.to));
        }
        if (options.curve == compiler::FadeCC::CurveTypeLin)
        {
            com::Tween<double, com::TweenTypeLin> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else if (options.curve == compiler::FadeCC::CurveTypeQuad)
        {
            com::Tween<double, com::TweenTypeQuad> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else if (options.curve == compiler::FadeCC::CurveTypeCub)
        {
            com::Tween<double, com::TweenTypeCub> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else if (options.curve == compiler::FadeCC::CurveTypeQuart)
        {
            com::Tween<double, com::TweenTypeQuart> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else if (options.curve == compiler::FadeCC::CurveTypeQuint)
        {
            com::Tween<double, com::TweenTypeQuint> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else if (options.curve == compiler::FadeCC::CurveTypeExp)
        {
            com::Tween<double, com::TweenTypeExp> tween(options.from, options.to, duration);
            _perform(track, startPos, channel,  tween, duration, options.type);
        }
        else
        {
            FM_THROW(compiler::Exception, "unsupported curve type: " + options.type);
        }
    }
}