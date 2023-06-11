#pragma once

#include <com/IRegisterable.h>
#include <com/midi.hpp>
#include <documentModel/objects/ConductionSheetDef.h>

namespace conductor
{
    typedef std::pair<com::Byte, com::Byte> TimeSignature;
    struct EventWithMetaInfo
    {
        com::midi::Event *midiEvent = nullptr;
        com::midi::Event *noteOff = nullptr;
        com::midi::Event *predecessorNoteOn = nullptr;  // of same channel & pitch
        com::midi::Event *predecessorNoteOff = nullptr; // of same channel & pitch
        com::midi::Event unmodifiedOriginalMidiEvent;
        com::midi::Event unmodifiedOriginalNoteOff;
        TimeSignature timeSignature = {4, 4};
        com::Ticks barNumber = 0; 
    };
    class ISelector : public com::IRegisterable
    {
    public:
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const = 0;
        virtual ~ISelector() = default;
    };
}
