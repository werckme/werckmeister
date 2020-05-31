#include "InstrumentConfigVelocityRemap.h"
#include <compiler/context/AContext.h>

namespace sheet {
    namespace compiler {
        void InstrumentConfigVelocityRemap::execute(AContext* context)
        {
            // #74.2
            // auto value         = parameters[argumentNames.XYZ].value<int>();
            // // velocity overrides
            // auto assignIfSet = [&argsExceptFirst, instrumentDef, this](const fm::String &expression){
            //      // #74.2 TODO
            //      // auto foundValue = fm::getArgumentValue<int>(expression, argsExceptFirst);
            //      // if (!foundValue.first) {
            //      //      return;
            //      // }
            //      // if (foundValue.second < 0 || foundValue.second > 100) {
            //      //      FM_THROW(Exception, "invalid value for: " + expression);
            //      // }
            //      // auto exprValue = getExpression(expression);
            //      // instrumentDef->velocityOverride[exprValue] = foundValue.second;
            // };
            // for(const auto &keyValue : expressionMap_) {
            //      assignIfSet(keyValue.first);
            // }
        }
    }
}