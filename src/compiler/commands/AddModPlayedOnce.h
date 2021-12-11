#ifndef ADDMODPLAYEDONCE_HPP
#define ADDMODPLAYEDONCE_HPP

#include "AddMod.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        /// <command name="modOnce" where="track">
        /// Like [mod](#mod). But with the difference, that the loaded mod will be only executed once for the following event.
        /// ### examples
        /// `/modOnce: arpeggio/`
        /// </command>
        /// <param name="use" position="0" type="text">The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>
        class AddModPlayedOnce : public AddMod
        {
        public:
            virtual void execute(IContextPtr ) override;
        };
    }
}

#endif