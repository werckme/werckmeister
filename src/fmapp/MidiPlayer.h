#ifndef FMAPP_MIDIPLAYER_HPP
#define FMAPP_MIDIPLAYER_HPP

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>

namespace fmapp {
    class MidiPlayer : public IDocumentWriter {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        fm::ILoggerPtr             _logger;
        
    public:
        MidiPlayer(
            ICompilerProgramOptionsPtr  programOptions, 
            fm::midi::MidiPtr           midiFile,
            fm::ILoggerPtr              logger) 
            : _programOptions(programOptions), 
              _midifile      (midiFile),
              _logger        (logger)
        {}
        virtual void write(sheet::DocumentPtr document);
        virtual ~MidiPlayer() = default;
    private:
        void printElapsedTime(fm::Ticks elapsed);
        void execLoop(sheet::DocumentPtr document);
        
    };
}
#endif