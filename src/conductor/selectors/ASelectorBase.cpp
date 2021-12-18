#include "ASelectorBase.h"
#include <com/midi.hpp>
#include <compiler/error.hpp>
#include <algorithm>

namespace conductor
{
     size_t ASelectorBase::getLoopIndex(const EventWithMetaInfo &evm) const
     {
        const auto midiEvent = *evm.noteOn;
        const compiler::EventInformation *documentEventInfos = _eventInformationServer->find(midiEvent);
        if (documentEventInfos == nullptr)
        {
            FM_THROW(compiler::Exception, "did not found document event related to midi event: " + midiEvent.toString());
        }
        const auto &midiPositionsOfDocumentEvent = documentEventInfos->positions;
        bool numberOfPositionsIsEven = midiPositionsOfDocumentEvent.size() % 2 == 0;
        if (!numberOfPositionsIsEven || midiPositionsOfDocumentEvent.empty())
        {
            FM_THROW(compiler::Exception, "invalid informations for this midi event: " + midiEvent.toString());
        }
        auto absPosition = midiEvent.absPosition();
        auto iteratorOfPosition = std::find(midiPositionsOfDocumentEvent.begin(), midiPositionsOfDocumentEvent.end(), absPosition);
        size_t loopIndex = std::distance(midiPositionsOfDocumentEvent.begin(), iteratorOfPosition);
        loopIndex/= 2; // exclude note off events
        return loopIndex;
     }
}