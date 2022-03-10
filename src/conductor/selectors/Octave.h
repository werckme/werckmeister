#pragma once

#include "ISelector.h"

namespace conductor
{
    /// <selector name="octave">
    ///     Selects any event where its pitch is in the range of a given octave. The middle c is located in octave 5.
    /// ```
    /// octave(4) {...}
    /// ```
    /// </selector>
    /// <param name="an octave number from 0 to 10" type="number+"></param>
    class Octave : public ISelector
    {
    public:
        Octave() = default;
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Octave() = default;
    };
}
