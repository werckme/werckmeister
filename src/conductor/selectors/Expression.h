#pragma once

#include "ISelector.h"
#include <com/IDefinitionsServer.h>

namespace documentModel
{
    namespace conductor
    {
        /// <selector name="expression">
        ///     Select events via its applied expressions.
        /// ```
        /// expression(p) {...}
        /// expression(p f ff) {...}
        /// ```
        /// </selector>
        /// <param name="expression" type="expression+"></param>
        class Expression : public ISelector
        {
        public:
            Expression() {}
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~Expression() = default;
        };
    }
}

