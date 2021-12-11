#ifndef SHEET_MARK_HPP
#define SHEET_MARK_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="mark" where="voice">
        ///     adds a mark to the voice. Us it in combination with [jump](manual/#jump).
        /// ### examples
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
        /// <param name="name"  position="0" type="word">the name of the mark</param>   
        class Mark : public ACommand
        {
        public:
            com::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF(argumentNames.Mark.Name, 	0), 
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr);
        };
    }
}

#endif