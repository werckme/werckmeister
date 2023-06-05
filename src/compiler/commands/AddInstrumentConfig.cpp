#include "AddInstrumentConfig.h"
#include <compiler/context/IContext.h>
#include <compiler/metaCommands.h>
#include <com/tools.h>
#include <compiler/error.hpp>
#include <com/werckmeister.hpp>
#include "ICanSpecifyInstrument.h"
#include <iostream>

namespace compiler
{

    const std::vector<com::String> AddInstrumentConfig::SupportedConfigCommands = {
        SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME,
        SHEET_META__SET_INSTRUMENT_CONFIG_PAN,
        SHEET_META__SET_VOICING_STRATEGY,
        SHEET_META__SET_MOD,
        SHEET_META__VELOCITY_REMAP,
        SHEET_META__INSTRUMENT_CONFIG_SET_CC
    };

    void AddInstrumentConfig::execute(IContextPtr context)
    {
        if (this->_configCommands.empty())
        {
            return;
        }
        auto uname = parameters[argumentNames.InstrumentConf.ForInstrument].value<com::String>();
        auto instrumentDef = context->getInstrumentDef(uname);
        if (instrumentDef == nullptr)
        {
            FM_THROW(Exception, "instrument not found: " + uname);
        }
        for (auto configCommand : this->_configCommands)
        {
            const auto &name = configCommand.first;
            auto commandObject = configCommand.second;
            auto canSpecifyInstrument = std::dynamic_pointer_cast<ICanSpecifyInstrument>(commandObject);
            if (!canSpecifyInstrument)
            {
                return;
            }
            canSpecifyInstrument->setInstrument(instrumentDef);
            try
            {
                commandObject->execute(context);
            }
            catch (const std::exception &ex)
            {
                com::StringStream ss;
                ss << "failed to process: " << name << std::endl;
                ss << ex.what();
                FM_THROW(Exception, ss.str());
            }
        }
    }

    void AddInstrumentConfig::setArguments(const Arguments &args)
    {
        auto &wm = com::getWerckmeister();
        Base::setArguments(args);
        auto uname = parameters[argumentNames.InstrumentConf.ForInstrument].value<com::String>();
        if (args.size() < 3)
        {
            FM_THROW(Exception, "not enough values for " + com::String(SHEET_META__SET_INSTRUMENT_CONFIG) + ": " + uname);
        }
        auto argsMappedByKeyword = com::mapArgumentsByKeywords(args, SupportedConfigCommands);
        for (auto configCommandName : SupportedConfigCommands)
        {
            auto argsRange = argsMappedByKeyword.equal_range(configCommandName);
            if (argsRange.first == argsRange.second)
            {
                continue;
            }
            auto it = argsRange.first;
            for (; it != argsRange.second; ++it)
            {
                const auto &args = it->second;
                auto commandStr = InstrumentConfigCommandName(configCommandName);
                auto commandObject = wm.solveOrDefault<ACommand>(commandStr);
                if (!commandObject)
                {
                    continue;
                }
                commandObject->setArguments(args);
                _configCommands.push_back(std::make_pair(configCommandName, commandObject));
            }
        }
    }
}