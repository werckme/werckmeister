#ifndef SEL_EXPRESSION_H
#define SEL_EXPRESSION_H

#include "ISelector.h"
#include <fm/IDefinitionsServer.h>

namespace sheet
{
    namespace conductor
    {
        /// <selector name="pitch">
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

#endif