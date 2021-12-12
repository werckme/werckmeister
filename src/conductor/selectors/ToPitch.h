#pragma once

#include "ISelector.h"
#include <com/IDefinitionsServer.h>

namespace conductor
{
    /// <selector name="toPitch">
    ///     Selects any event where its pitch is lower or equal than the given pitch.
    /// see also: [fromPitch](#fromPitch), [pitch](#pitch).
    /// ```
    /// toPitch(c,) {...}
    /// ```
    /// </selector>
    /// <param name="pitch" type="pitch"></param>
    class ToPitch : public ISelector
    {
    public:
        ToPitch(com::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
        virtual bool isMatch(const ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToPitch() = default;

    private:
        com::IDefinitionsServerPtr _definitionServer;
    };
}
