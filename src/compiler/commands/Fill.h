#ifndef SHEET_FILL_HPP
#define SHEET_FILL_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="fill" where="accomp">
        /// Plays a template only once. Is also able to replace the performance of another template during its playback.
        /// Useful for fill ins.
        /// ```
        ///   /fill: myDrumfill _replace=myDrums/
        /// ```
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