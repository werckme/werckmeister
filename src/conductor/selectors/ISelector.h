#ifndef ISELECTOR_H
#define ISELECTOR_H

#include <fm/IRegisterable.h>
#include <fm/midi.hpp>
#include <sheet/objects/ConductionSheetDef.h>

namespace sheet
{
    namespace conductor
    {
        typedef std::pair<fm::Byte, fm::Byte> TimeSignature;
        struct EventWithMetaInfo
        {
            fm::midi::Event* noteOn;
            fm::midi::Event* noteOff;
            TimeSignature timeSignature;
            fm::String instrumentName;
        };
        class ISelector : public fm::IRegisterable
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const EventWithMetaInfo&) const = 0;
            virtual ~ISelector() = default;
        };
    }
}

#endif