#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <IPlayerProgramOptions.h>
#include <fm/ILogger.h>
#include <fmapp/IPlayerLoopVisitor.h>
#include <vector>
#include <fmapp/DiContainerWrapper.h>
#include <memory>
#include "midiplayerClient.h"
#include "midiProvider.h"
#include "rtmidiBackend.h"
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
        typedef MidiplayerClient<RtMidiBackend, fmapp::MidiProvider, TimerImpl> MidiplayerImpl;
        enum State { Stopped, Playing };
    private:
        IPlayerProgramOptionsPtr   _programOptions;
        fm::midi::MidiPtr          _midifile;
        LoopVisitors               _loopVisitors;
        fm::ILoggerPtr             _logger;
    public:
        MidiPlayer(
            ICompilerProgramOptionsPtr  programOptions, 
            fm::midi::MidiPtr           midiFile,
            LoopVisitors                loopVisitors,
            fm::ILoggerPtr              logger) 
            : ADocumentWriter(logger),
              _programOptions(std::dynamic_pointer_cast<IPlayerProgramOptions>(programOptions)),
              _midifile      (midiFile),
              _loopVisitors  (loopVisitors),
              _logger        (logger)
        {

        }
        void listDevices(std::ostream&);
        virtual void write(sheet::DocumentPtr document);
        virtual ~MidiPlayer() = default;
        /**
         * stops the player and returns the last tick position
         */
        virtual fm::Ticks stop();
    private:
        enum VisitorMessage { Loop, BeginLoop, EndLoop };
        void execLoop(sheet::DocumentPtr document);
        void visitVisitors(VisitorMessage, fm::Ticks elapsed);
        State state = Stopped;
        MidiplayerImpl _midiPlayerImpl;
        
    };
    typedef std::shared_ptr<MidiPlayer> MidiPlayerPtr;
}
#endif