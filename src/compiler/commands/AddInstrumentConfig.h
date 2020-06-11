#ifndef ADDINSTRUMENTCONFIG_HPP
#define ADDINSTRUMENTCONFIG_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <list>
#include <memory>


namespace sheet {
    namespace compiler {
       
        inline fm::String InstrumentConfigCommandName(const fm::String &cmdName) 
        {
            return "InstrumentConfigCommandPrefix_" + cmdName;
        }

        class AddInstrumentConfig : public ACommand
        {
        public:
            typedef ACommand Base;
            static const std::vector<fm::String> SupportedConfigCommands;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentConf.ForInstrument, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContext*);
            virtual void setArguments(const Arguments &args) override;
        private:
            typedef fm::String CommandName;
            typedef std::pair<CommandName, std::shared_ptr<ACommand>> ConfigCommand;
            typedef std::list<ConfigCommand> ConfigCommands;
            ConfigCommands _configCommands;
        };
    }
}

#endif