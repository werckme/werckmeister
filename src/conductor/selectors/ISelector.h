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
            fm::midi::Event* noteOn = nullptr;
            fm::midi::Event* noteOff = nullptr;
            fm::midi::Event* predecessorNoteOn = nullptr; // of same channel & pitch
            fm::midi::Event* predecessorNoteOff = nullptr; // of same channel & pitch
            TimeSignature timeSignature = {4, 4};
            fm::String instrumentName;
            fm::Ticks barNumber = 0;
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