#ifndef SHEET_SETVOLUME_HPP
#define SHEET_SETVOLUME_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="volume" where="track">
        ///     set the volume of the current track
        /// ### examples
        /// **positional:** 
        /// `/volume: 50/`
        /// **named:**
        /// `/volume: _to=50/`   
        /// </command>
        /// <param name="to" position="0" type="0..100">The volume value.</param>
        class SetVolume : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetVolume.SetTo, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif