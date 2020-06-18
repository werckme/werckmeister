#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include <fmapp/IPlayerLoopVisitor.h>
#include <vector>
#include <fmapp/DiContainerWrapper.h>
#include <memory>

namespace fmapp {
    class MidiPlayer : public IDocumentWriter {
    public:
        typedef DiContainerWrapper<IPlayerLoopVisitorPtr> LoopVisitors;
        enum State { Stopped, Playing, Paused };
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        LoopVisitors               _loopVisitors;
        fm::ILoggerPtr             _logger;
    public:
        MidiPlayer(
            ICompilerProgramOptionsPtr  programOptions, 
            fm::midi::MidiPtr           midiFile,
            LoopVisitors                loopVisitors,
            fm::ILoggerPtr              logger) 
            : _programOptions(programOptions), 
              _midifile      (midiFile),
              _loopVisitors  (loopVisitors),
              _logger        (logger)
        {
        }
        virtual void write(sheet::DocumentPtr document);
        virtual ~MidiPlayer() = default;
        virtual void pause();
        virtual void resume();
    private:
        void execLoop(sheet::DocumentPtr document);
        void visitVisitors(fm::Ticks elapsed);
        State state = Stopped;
        fm::Ticks pausePosition = 0;
        
    };
    typedef std::shared_ptr<MidiPlayer> MidiPlayerPtr;
}
#endif