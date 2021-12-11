#pragma once

#include "ISelector.h"

namespace documentModel
{
    namespace conductor
    {
        /// <selector name="fromBeat">
        ///Selects an event where its beat time is equal or greater.
        // see also: [toBeat](#toBeat), [onBeat](#onBeat).
        /// ```
        /// fromBeat(1) {...}
        /// ```
        /// </selector>
        /// <param name="beat" type="quarters"></param>
        class FromBeat : public ISelector
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromBeat() = default;
        };
    }
}

