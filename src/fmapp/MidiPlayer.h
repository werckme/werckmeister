#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <IPlayerProgramOptions.h>
#include <com/ILogger.h>
#include <fmapp/IPlayerLoopVisitor.h>
#include <vector>
#include <fmapp/DiContainerWrapper.h>
#include <memory>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "MidiBackendContainer.h"
#include <ostream>
#include "ADocumentWriter.h"

#ifdef SHEET_USE_BOOST_TIMER
#include "fmapp/boostTimer.h"
typedef fmapp::BoostTimer TimerImpl;
#else
#include "fmapp/os.hpp"
typedef fmapp::os::MMTimer TimerImpl;
#endif


namespace fmapp {
    class MidiPlayer : public ADocumentWriter {
    public:
        typedef DiContainerWrapper<IPlayerLoopVisitorPtr> LoopVisitors;
        typedef MidiplayerClient<MidiBackendContainer, fmapp::MidiProvider, TimerImpl> MidiplayerImpl;
        enum State { Stopped, Playing };
    private:
        IPlayerProgramOptionsPtr   _programOptions;
        com::midi::MidiPtr          _midifile;
        LoopVisitors               _loopVisitors;
        com::ILoggerPtr             _logger;
    public:
        MidiPlayer(
            ICompilerProgramOptionsPtr  programOptions, 
            com::midi::MidiPtr           midiFile,
            LoopVisitors                loopVisitors,
            com::ILoggerPtr              logger) 
            : ADocumentWriter(logger),
              _programOptions(std::dynamic_pointer_cast<IPlayerProgramOptions>(programOptions)),
              _midifile      (midiFile),
              _loopVisitors  (loopVisitors),
              _logger        (logger)
        {
            initMidiBackends();
        }
        void listDevices(std::ostream&);
        virtual void write(sheet::DocumentPtr document);
        virtual ~MidiPlayer() = default;
        /**
         * stops the player and returns the last tick position
         */
        virtual com::Ticks stop();
    private:
        enum VisitorMessage { Loop, BeginLoop, EndLoop };
        void initMidiBackends();
        void initFluidSynthInstances();
        void execLoop(sheet::DocumentPtr document);
        void visitVisitors(VisitorMessage, com::Ticks elapsed);
        State state = Stopped;
        MidiplayerImpl _midiPlayerImpl;
        
    };
    typedef std::shared_ptr<MidiPlayer> MidiPlayerPtr;
}
#endif