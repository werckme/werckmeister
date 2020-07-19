#ifndef SETSPIELANWEISUNGPLAYEDONCE_HPP
#define SETSPIELANWEISUNGPLAYEDONCE_HPP

#include "SetSpielanweisung.h"


namespace sheet {
    namespace compiler {
        /// <command name="doOnce" where="track">
        /// Like [do](#do). But with the difference, that the loaded mod will be only executed once for the following event.
        /// ### examples
        /// **positional:** 
        /// `/doOnce: arpeggio/`
        /// **named:**
        /// `/doOnce: _use=arpeggio/`  
        /// </command>
        /// <param name="use" position="0" type="word">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>

        class SetSpielanweisungPlayedOnce : public SetSpielanweisung
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

#endif