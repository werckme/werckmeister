#ifndef SETVOICINGSTRATEGY_HPP
#define SETVOICINGSTRATEGY_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <compiler/voicings/VoicingStrategy.h>

namespace sheet {
    namespace compiler {
        /// <command name="voicingStrategy" where="track">
        /// Adds a modification to the track.
        /// ### examples
        /// **positional:** 
        /// `/voicingStrategy: asNotated/` 
        /// **named:**
        /// `/voicingStrategy: _use=asNotated/`  
        /// Voicing strategies can be external lua scripts, or one of theese internal strategies:
        /// * asNotated
        /// * simpleGuitar
        /// </command>
        /// <param name="use" position="0" type="text">The name of the strategy to load. This is the only "unique" parameter for this command. All further parameters are specific to its related strategy.</param>

        class SetVoicingStrategy : public ACommand
        {
        public:
            typedef ACommand Base;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVoicingStrategy.Use, 	    0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void setArguments(const Arguments &args) override;
            virtual void execute(IContextPtr );
        protected:
            VoicingStrategyPtr voicingStrategy = nullptr;
        };
    }
}

#endif