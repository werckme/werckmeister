#ifndef SEL_TO_PITCH_H
#define SEL_TO_PITCH_H

#include "ISelector.h"
#include <fm/IDefinitionsServer.h>

namespace sheet
{
    namespace conductor
    {
        /// <selector name="toPitch">
        ///     Selects any event where its pitch is lower or equal than the given pitch.
        /// see also: [fromPitch](#fromPitch), [pitch](#pitch).
        /// ```
        /// toPitch(c,) {...}
        /// ```
        /// <param name="pitch" type="pitch"></param>
        /// </selector>
        class ToPitch : public ISelector
        {
        public:
            ToPitch(fm::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~ToPitch() = default;
        private:
            fm::IDefinitionsServerPtr _definitionServer;
        };
    }
}

#endif