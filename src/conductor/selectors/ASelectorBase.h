#pragma once

#include "ISelector.h"
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    class ASelectorBase : public ISelector
    {
    public:
        ASelectorBase(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer)
        {
        }
       virtual ~ASelectorBase() = default;
       /**
        * @brief Get the loop index of a midi event.
        * For example here `c d e f :|`
        * these notes will be peformed two times.
        * getLoopIndex(midiEvent) will tell you which loop iteration the midi note belongs to.
        * @throws if loop index couldn't be determined.
        * @return size_t 
        */
       size_t getLoopIndex(const EventWithMetaInfo &evm) const;
    protected:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
