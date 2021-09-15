#ifndef SEL_PITCH_H
#define SEL_PITCH_H

#include "ISelector.h"
#include <fm/IDefinitionsServer.h>

namespace sheet
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
            Pitch(fm::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~Pitch() = default;
        private:
            fm::IDefinitionsServerPtr _definitionServer;
        };
    }
}

#endif