#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>

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
        ToPitch(compiler::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~ToPitch() = default;

    private:
        compiler::IDefinitionsServerPtr _definitionServer;
    };
}
