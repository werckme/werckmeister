#include "Fill.h"
#include <compiler/context/IContext.h>
#include <com/common.hpp>

namespace documentModel {
    namespace compiler {
        void Fill::execute(IContextPtr  context)
        {
            // does nothing
            // will be processed via SheetTemplateRenderer
        }
        com::String Fill::templateName()
        {
            auto name = parameters[argumentNames.Fill.Template].value<com::String>();
            return name;
        }
        com::String Fill::replaceTemplateName()
        {
            auto name = parameters[argumentNames.Fill.Replace].value<com::String>();
            return name;
        }
    }
}