#ifndef ADDINSTRUMENTCONFIG_HPP
#define ADDINSTRUMENTCONFIG_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <list>
#include <memory>

namespace sheet {
    namespace compiler {
        class AddInstrumentConfig : public ACommand
        {
        public:
            typedef ACommand Base;
            static const std::vector<fm::String> SupportedConfigCommands;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentConf.ForInstrument, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(AContext*);
            virtual void setArguments(const Arguments &args) override;
        private:
            typedef std::list<std::shared_ptr<ACommand>> ConfigCommands;
            ConfigCommands _configCommands;
        };
    }
}

#endif