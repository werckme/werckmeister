#ifndef ADDMOD_HPP
#define ADDMOD_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace sheet {
    namespace compiler {
        /// <command name="mod" where="track">
        /// Adds a modification to the track. Every `mod` statement adds a further modification to the related voice.
        /// If you want to use ony one modification at the time use [do](#do) instead.
        /// ### examples
        /// **positional:** 
        /// `/mod: arpeggio/`
        /// **named:**
        /// `/mod: _use=arpeggio/`  
        /// Mods can be external lua scripts, or one of theese internal mods:
        /// #### Bend
        /// Performs a pitch bend from or to the pitch of a note. The value range is 0..100 where 50 means the pichbend center.
        /// ```language=Werckmeister,type=single,tempo=140
        ////doOnce: bend _from=0/ c1 | /doOnce: bend _to=0/ c1
        /// ```
        /// #### Arpeggio
        /// Performs an arpeggio over a given chord.
        /// **Arpeggio is deprecated. Use the lua mod [myArpeggio](#myarpeggio) instead.**
        /// </command>
        /// <param name="use" position="0" type="text">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>
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