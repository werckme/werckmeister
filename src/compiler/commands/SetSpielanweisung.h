#ifndef SETSPIELANWEISUNG_HPP
#define SETSPIELANWEISUNG_HPP

#include "AddMod.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace sheet {
    namespace compiler {
        /// <command name="do" where="track">
        /// <![CDATA[
        /// Such as [mod](#mod)&nbsp;`do` adds a modification to the track. 
        /// The difference is: every `mod` command adds a further modification to a voice.
        /// `do` replaces the previous modification. 
        /// ### examples
        /// **positional:** 
        /// `/do: arpeggio/`
        /// **named:**
        /// `/do: _use=arpeggio/`
        /// ]]>
        /// </command>
        /// <param name="use" position="0" type="text">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>
        class SetSpielanweisung : public AddMod
        {
        public:
            virtual void execute(IContextPtr) override;
        };
    }
}

#endif