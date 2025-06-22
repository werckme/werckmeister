#include "Cc.h"
#include <regex>
#include <string>
#include <iostream> 
#include <compiler/commands/SetCC.h>
#include <charconv> 

namespace conductor
{

    namespace 
    {
        typedef compiler::SetCC::ControllerNumberMap ControllerNumberMap;

        ControllerNumberMap _createControllerMap()
        {
            ControllerNumberMap map(compiler::SetCC::getControllerNumberMap());
            for(int i=0; i < 127; ++i)
            {
                map.insert({"cc" + std::to_string(i), i});
            }
            return map; 
        }
        ControllerNumberMap _controllerNames = _createControllerMap();
    }

    Cc::Cc(com::midi::MidiPtr midi) : midi(midi) 
    {
    }

    void Cc::perform(const Events &events) const
    {
        if (declaration.unit == documentModel::ConductionRule::Declaration::UnitPercent)
        {
            FM_THROW(compiler::Exception, "the unit precent isn't supported by cc");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationAdd)
        {
            FM_THROW(compiler::Exception, "the add operator isn't supported by cc");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationSubstract)
        {
            FM_THROW(compiler::Exception, "the substract operator isn't supported by cc");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpAdd)
        {
            FM_THROW(compiler::Exception, "the follow up add operator isn't supported by cc");
        }
        if (declaration.operation == documentModel::ConductionRule::Declaration::OperationFollowUpSubstract)
        {
            FM_THROW(compiler::Exception, "the follow up substract operator isn't supported by cc");
        }
        std::string input = declaration.strValue();
        std::regex pattern("(\\w+\\d*)\\((\\d+)\\)");
        std::smatch match;
        if (!std::regex_search(input, match, pattern))
        {
            FM_THROW(compiler::Exception, "invalid cc expression: " + input);
        }
        auto controllerStr = match[1].str();
        auto strValue = match[2].str();
        auto ccIt = _controllerNames.find(controllerStr);
        if (ccIt == _controllerNames.end())
        {
             FM_THROW(compiler::Exception, "invalid cc name: '" + controllerStr + "'. Please find here a complete list of supported cc names https://werckme.github.io/manual#cc");

        }
        auto ccNr = ccIt->second;
        int ccValue = 0;
        auto [ptr, ec] = std::from_chars(strValue.data(), strValue.data() + strValue.size(), ccValue);
        if (ec != std::errc()) {
            FM_THROW(compiler::Exception, "invalid cc value: " + strValue);
        }
        auto noteOn = events.midiEvent;
        auto eventPosition = noteOn->absPosition();
        auto track = noteOn->parentTrack;
        assert(track != nullptr);
        if (ccValue < 0 || ccValue > 127) 
        {
            FM_THROW(compiler::Exception, "invalid cc value (0..127): " + std::to_string(ccValue));
        }
        auto ccEvent = com::midi::Event::CCValue(noteOn->channel(), (com::Byte)ccNr, (com::Byte)ccValue);
        ccEvent.absPosition(eventPosition);
        track->events().add(ccEvent);
    }
}