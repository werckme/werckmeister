#pragma once

#include "ISelector.h"
#include <compiler/IDefinitionsServer.h>
#include <compiler/IEventInformationServer.h>

namespace conductor
{
    /// <selector name="chord">
    ///     Selects a MIDI event which was created during rendering a specific chord.
    /// ```\n
    /// chord(Cmaj7) {...}\n
    /// ```
    /// It is also possible to select "chord qualities". For example, if you want to select
    /// every 7th chord you write:
    /// ```\n
    /// chord(X7) {...}\n
    /// ```
    /// </selector>
    /// <param name="chord name" type="string+"></param>
    class Chord : public ISelector
    {
    public:
        Chord(compiler::IEventInformationServerPtr eventInformationServer) : _eventInformationServer(eventInformationServer) {}
        virtual bool isMatch(const documentModel::ConductionSelector::Arguments &, const EventWithMetaInfo &) const override;
        virtual ~Chord() = default;

    private:
        compiler::IEventInformationServerPtr _eventInformationServer;
    };
}
