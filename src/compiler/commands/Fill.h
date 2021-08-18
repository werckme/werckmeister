#ifndef SHEET_FILL_HPP
#define SHEET_FILL_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        /// <command name="fill" where="accomp">
        ///     TBD
        /// </command>
        /// <param name="template"  position="0" type="text">the name of the template to fill in</param>
        /// <param name="replace"  type="text">the name of the template to be replaced by the fill</param>   
        class Fill : public ACommand
        {
        public:
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF(argumentNames.Fill.Template, 	0),
                FM_PARAMETER_DEF(argumentNames.Fill.Replace, 	1), 
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr);
            fm::String templateName();
            fm::String replaceTemplateName();
        };
    }
}

#endif