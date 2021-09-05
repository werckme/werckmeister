#ifndef SHEET_DEFINEINSTRUMENTSECTION_HPP
#define SHEET_DEFINEINSTRUMENTSECTION_HPP

#include "AMidiContextCommand.h"
#include <compiler/argumentNames.h>
#include <list>

namespace sheet {
    namespace compiler {
        /// <command name="instrumentSection" where="document">
        /// Combines several instruments into a new one.
        /// ### examples
        /// **positional:** 
        /// `instrumentSection: bass piano ebass`; 
        /// **named:**
        /// `instrumentSection: _setName=bass piano ebass`; 
        /// **a complete example**
        /// define an device, an instrument and set it to a track.
        /// see [instrumentDef](#instrumentDef), [instrument](#instrument), [device](#device)
        /// <![CDATA[
        /// ```language=Werckmeister\n
        /// tempo: 120;\n
        /// device: MyDevice  midi _usePort=0;\n
        /// instrumentDef:piano    _onDevice=MyDevice  _ch=0 _pc=0;\n
        /// instrumentDef:guitar   _onDevice=MyDevice _ch=1  _pc=29;\n
        /// instrumentDef:organ   _onDevice=MyDevice _ch=2  _pc=16;\n
        /// instrumentSection: myNewInstrument piano guitar organ;\n
        /// \n
        /// [\n
        /// instrument: myNewInstrument;\n
        /// {\n
        ///  a,,1 | a, | d#,~ | &\n
        /// }\n
        /// ]\n
        /// ```
        /// ]]>
        /// </command>
        /// <param name="setName"  position="0" type="text">An arbitary name.</param>
        class DefineInstrumentSection : public AMidiContextCommand
        {
        public:
            typedef AMidiContextCommand Base;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentSection.WithName, 	0),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
            virtual void setArguments(const Arguments &args) override;
            std::list<fm::String> sectionInstruments;
        };
    }
}

#endif