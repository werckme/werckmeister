#ifndef SHEET_JUMP_HPP
#define SHEET_JUMP_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="jump" where="voice">
        ///     Jumps to a previous defined mark See [mark](manual/#mark).
        /// ### examples
        /// ```
        /// device: _usePort=0 _setName=MyDevice  _isType=midi;
        /// instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;
        /// [
        /// instrument: piano;
        /// {
        ///     /signature: 5 4/
        ///     /mark: A/       -- valid & unique to this voice
        ///     c d e f g |
        ///     /jump: A/       -- jumps to A once
        ///     r1 r4
        /// }
        /// ]
        /// ```
        /// </command>
        /// <param name="to"      position="0" type="word">the destination marker</param>  
        /// <param name="ignore"  position="" optional="1" type="0..100">Ignores the jump N times</param>   
        /// <param name="repreat" position="" optional="1" type="0..100">Repeats the jump N times. (A repeat value of 1 performs 2 jumps)</param>   
        class Jump : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF            (argumentNames.Jump.To, 	    0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.Jump.Ignore, 	1, 0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.Jump.Repeat, 	2, 0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

#endif