
#pragma once

#include "ASelectorBase.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="withTag">
    ///     Selects any event which has a tag assigned matching to the given tag.
    /// #### example, select the event with the tag "myTag" assigned:
    /// ```\n
    /// withTag(myTag) {...}\n
    /// ```\n
    /// ```\n
    /// c d e "myTag"@f |\n
    /// ```
    /// </selector>
    /// <param name="tag" type="text"></param>
    class WithTag : public ASelectorBase
    {
    public:
        typedef ASelectorBase Base;
        WithTag(compiler::IEventInformationServerPtr eventInformationServer) : Base(eventInformationServer)
        {
        }
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~WithTag() = default;
    };
}
