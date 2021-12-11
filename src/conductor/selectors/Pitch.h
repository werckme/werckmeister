#pragma once

#include "ISelector.h"
#include <com/IDefinitionsServer.h>

namespace documentModel
{
    namespace conductor
    {
        /// <selector name="pitch">
        ///     Selects any event where its pitch is equal to one of the given pitches.
        /// see also: [fromPitch](#fromPitch), [toPitch](#toPitch).
        /// ```
        /// pitch(c) {...}
        /// pitch(c, c c') {...}
        /// ```
        /// </selector>
        /// <param name="pitch" type="pitch+"></param>
        class Pitch : public ISelector
        {
        public:
            Pitch(com::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~Pitch() = default;
        private:
            com::IDefinitionsServerPtr _definitionServer;
        };
    }
}

