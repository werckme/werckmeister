#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="phrase">
    ///     Selects a MIDI event which was created during rendering a phrase.
    /// ```
    /// phrase(myPhrase1 myPhrase2) {...}
    /// ```
    /// </selector>
    /// <param name="name" type="text+">name(s) of a phrase</param>
    class Phrase : public ISelector
    {
    public:
        Phrase(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Phrase() = default;

    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
