#pragma once

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <IPlayerProgramOptions.h>
#include <com/ILogger.h>
#include <app/IPlayerLoopVisitor.h>
#include <vector>
#include <app/DiContainerWrapper.h>
#include <memory>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "MidiBackendContainer.h"
#include <ostream>
#include "ADocumentWriter.h"
#include <lua/PerformerScript.h>

#ifdef SHEET_USE_BOOST_TIMER
#include "app/boostTimer.h"
typedef app::BoostTimer TimerImpl;
#else
#include "app/os.hpp"
typedef app::os::MMTimer TimerImpl;
#endif

namespace app
{
    class MidiPlayer : public ADocumentWriter
    {
    public:
        typedef DiContainerWrapper<IPlayerLoopVisitorPtr> LoopVisitors;
        typedef MidiplayerClient<MidiBackendContainer, app::MidiProvider, TimerImpl> MidiplayerImpl;
        enum State
        {
            Stopped,
            Playing
        };

    private:
        IPlayerProgramOptionsPtr _programOptions;
        com::midi::MidiPtr _midifile;
        LoopVisitors _loopVisitors;
        com::ILoggerPtr _logger;
        lua::PerformerScriptPtr _performerScript;

    public:
        MidiPlayer(
            ICompilerProgramOptionsPtr programOptions,
            com::midi::MidiPtr midiFile,
            LoopVisitors loopVisitors,
            com::ILoggerPtr logger,
            lua::PerformerScriptPtr performerScript)
            : ADocumentWriter(logger),
              _programOptions(std::dynamic_pointer_cast<IPlayerProgramOptions>(programOptions)),
              _midifile(midiFile),
              _loopVisitors(loopVisitors),
              _logger(logger),
              _performerScript(performerScript)
        {
            initMidiBackends();
        }
        void listDevices(std::ostream &);
        virtual void write(documentModel::DocumentPtr document);
        virtual ~MidiPlayer() = default;
        /**
         * stops the player and returns the last tick position
         */
        virtual com::Ticks stop();

    private:
        enum VisitorMessage
        {
            Loop,
            BeginLoop,
            EndLoop
        };
        void initMidiBackends();
        void initFluidSynthInstancesIfNecessary();
        void execLoop(documentModel::DocumentPtr document);
        void visitVisitors(VisitorMessage, com::Ticks elapsed);
        State state = Stopped;
        MidiplayerImpl _midiPlayerImpl;
    };
    typedef std::shared_ptr<MidiPlayer> MidiPlayerPtr;
}
