#ifndef SEL_FROM_PITCH_H
#define SEL_FROM_PITCH_H

#include "ISelector.h"
#include <com/IDefinitionsServer.h>

namespace sheet
{
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
            FromPitch(com::IDefinitionsServerPtr definitionServer) : _definitionServer(definitionServer) {}
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const override;
            virtual ~FromPitch() = default;
        private: 
            com::IDefinitionsServerPtr _definitionServer;
        };
    }
}

#endif