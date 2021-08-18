#include "Fill.h"
#include <compiler/context/IContext.h>
#include <fm/common.hpp>

namespace sheet {
    namespace compiler {
        void Fill::execute(IContextPtr  context)
        {
            // does nothing
            // will be processed via SheetTemplateRenderer
        }
        fm::String Fill::templateName()
        {
            auto name = parameters[argumentNames.Fill.Template].value<fm::String>();
            return name;
        }
        fm::String Fill::replaceTemplateName()
        {
            auto name = parameters[argumentNames.Fill.Replace].value<fm::String>();
            return name;
        }
    }
}