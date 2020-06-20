#ifndef FMAPP_MIDIFILEWRITER_H
#define FMAPP_MIDIFILEWRITER_H

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include "ADocumentWriter.h"

namespace fmapp {
    class MidiFileWriter : public ADocumentWriter {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        fm::ILoggerPtr             _logger;
        
    public:
        MidiFileWriter(
            ICompilerProgramOptionsPtr  programOptions, 
            fm::midi::MidiPtr           midiFile,
            fm::ILoggerPtr              logger) 
            : ADocumentWriter(logger),
              _programOptions(programOptions), 
              _midifile      (midiFile),
              _logger        (logger)
        {}
        virtual void write(sheet::DocumentPtr document);
        virtual ~MidiFileWriter() = default;
    };
}

#endif