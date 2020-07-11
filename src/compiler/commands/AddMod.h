#ifndef ADDMOD_HPP
#define ADDMOD_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace sheet {
    namespace compiler {
        /// <command name="mod" where="track">
        /// Adds a modification to the track.
        /// ### examples
        /// **positional:** 
        /// `/mod: arpeggio/`
        /// **named:**
        /// `/mod: _use=arpeggio/`  
        /// Mods can be external lua scripts, or one of theese internal mods:
        /// #### Bend
        /// `/mod: bend _from=0/`
        /// #### Arpeggio
        /// `/mod: arpeggio/`
        /// </command>
        /// <param name="use" position="0" type="word">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>
        class AddMod : public ACommand
        {
        public:
            typedef ACommand Base;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.AddMod.Use, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
            virtual void setArguments(const Arguments &args) override;
            AModificationPtr theModification;
        };
    }
}

#endif