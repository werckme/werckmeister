#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>

namespace conductor
{
    /// <selector name="fromPitch">
    ///     Selects any event where its pitch is higher or equal than the given pitch.
    /// see also: [toPitch](#toPitch), [pitch](#pitch).
    /// ```
    /// fromPitch(c,) {...}
    /// ```
    /// </selector>
    /// <param name="pitch" type="pitch"></param>
    class FromPitch : public ISelector
    {
    public:
        FromPitch(compiler::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~FromPitch() = default;

    private:
        compiler::IDefinitionsServerPtr _definitionServer;
    };
}
