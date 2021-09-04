#ifndef SETINSTRUMENT_HPP
#define SETINSTRUMENT_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="instrument" where="trackConfig, track">
        /// Set or change the instrument of a track.
        /// ### examples
        /// **positional:** 
        /// `instrument: myInstrument;`
        /// **named:**
        /// `instrument: _use=myInstrument;`
        /// **set an instrument for a track**
        /// ```
        /// [ \n
        /// instrument: myInstrument; \n
        /// { \n
        ///   c d e f \n
        /// } \n
        /// ] \n
        /// ```
        /// **change current instrument within a track**
        /// ```
        /// [ \n
        /// { \n
        ///   c d e f | \n
        ///   /instrument: myOtherInstrument/
        ///   c d e f
        /// } \n
        /// ] \n
        /// ```       
        /// </command>
        /// <param name="use" position="0" type="text">The instrument name.</param>
        class SetInstrument : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.SetInstrument.Use, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif