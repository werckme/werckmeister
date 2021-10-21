#ifndef SHEET_FILL_HPP
#define SHEET_FILL_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="fill" where="accomp">
        /// Plays a template only once. Is also able to replace the performance of another template during its playback.
        /// Useful for fill ins.
        /// <![CDATA[
        /// ```language=Werckmeister\n
        ///using "chords/default.chords";\n
        ///tempo: 90;\n
        ///device: MyDevice  midi _usePort=0;\n
        ///instrumentDef:drums    _onDevice=MyDevice  _ch=9 _pc=0;\n
        ///\n
        ///[\n
        ///type: template;\n
        ///name: drums.fill;\n
        ///instrument: drums;\n
        ///{\n
        ///  (c, & &)4 (b,, & &)4 (a,, & &)4 (g,, & &)4 |\n
        ///}\n
        ///]\n
        ///\n
        ///[\n
        ///type: template;\n
        ///name: drums.main;\n
        ///instrument: drums;\n
        ///{\n
        ///  r8 f#,, r & r & r & |\n
        ///}\n
        ///{\n
        ///   r4 d,, r4 & | \n
        ///}\n
        ///{\n
        ///  c,,4 r & r |\n
        ///}\n
        ///]\n
        ///\n
        ///[\n
        ///type: accomp;\n
        ///{\n
        /// /template: drums.main/\n
        /// C |\n
        /// /fill: drums.fill/ -- play fill and drum beat together\n
        /// C | C |\n
        /// /fill: drums.fill _replace="drums.main"/ -- play only the fill in\n
        /// C | C |\n
        ///}\n
        ///]\n
        /// ```
        /// ]]>
        /// </command>
        /// <param name="replace"  type="text">the name of the template to be replaced by the fill</param>   
        class Fill : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF(argumentNames.Fill.Template, 	        0    ),
                FM_PARAMETER_DEFAULT_DEF(argumentNames.Fill.Replace, 	1, "NO_VALID_FILL_REPLACE_VALUE"),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr);
            fm::String templateName();
            fm::String replaceTemplateName();
        };
    }
}

#endif