#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace documentModel {
    namespace compiler {
        /// <command name="jump" where="voice">
        ///     Jumps to a previous defined mark See [mark](manual/#mark).
        /// ### example
        /// <![CDATA[
        /// ```language=Werckmeister\n
        /// device: _usePort=0 _setName=MyDevice  _isType=midi;\n
        /// instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;\n
        /// [\n
        /// instrument: piano;\n
        /// {\n
        ///     /signature: 5 4/\n
        ///     /mark: A/       -- valid & unique to this voice\n
        ///     c4 d e f g |\n
        ///     /jump: A/       -- jumps to A once\n
        ///     a b c'2\n
        /// }\n
        /// ]\n
        /// ```
        /// ]]>
        /// </command>
        /// <param name="to"      position="0" type="text">the destination marker</param>  
        /// <param name="ignore"  position="" optional="1" type="0..100">Ignores the jump N times</param>   
        /// <param name="repeat" position="" optional="1" type="0..100">Repeats the jump N times. (A repeat value of 1 performs 2 jumps)</param>   
        class Jump : public ACommand
        {
        public:
            com::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF            (argumentNames.Jump.To, 	    0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.Jump.Ignore, 	1, 0),
                FM_PARAMETER_DEFAULT_DEF    (argumentNames.Jump.Repeat, 	2, 0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
        };
    }
}

