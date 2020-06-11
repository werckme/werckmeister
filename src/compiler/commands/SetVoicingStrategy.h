#ifndef SETVOICINGSTRATEGY_HPP
#define SETVOICINGSTRATEGY_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <compiler/voicings/VoicingStrategy.h>

namespace sheet {
    namespace compiler {
        class SetVoicingStrategy : public ACommand
        {
        public:
            typedef ACommand Base;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVoicingStrategy.Use, 	    0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void setArguments(const Arguments &args) override;
            virtual void execute(IContext*);
        protected:
            VoicingStrategyPtr voicingStrategy = nullptr;
        };
    }
}

#endif