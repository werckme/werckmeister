#ifndef SHEET_MARK_HPP
#define SHEET_MARK_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="mark" where="voice">
        ///     adds a mark to the voice. Why you want to do that? See [jump](manual/#jump).
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
        /// <param name="name"  position="0" type="word">the name of the mark</param>   
        class Mark : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF(argumentNames.Mark.Name, 	0), 
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr);
        };
    }
}

#endif