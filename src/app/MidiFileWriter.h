#pragma once

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <com/ILogger.h>
#include "ADocumentWriter.h"

namespace app
{
    class MidiFileWriter : public ADocumentWriter
    {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        com::midi::MidiPtr _midifile;
        com::ILoggerPtr _logger;

    public:
        MidiFileWriter(
            ICompilerProgramOptionsPtr programOptions,
            com::midi::MidiPtr midiFile,
            com::ILoggerPtr logger)
            : ADocumentWriter(logger),
              _programOptions(programOptions),
              _midifile(midiFile),
              _logger(logger)
        {
        }
        virtual void write(documentModel::DocumentPtr document);
        virtual ~MidiFileWriter() = default;
    };
}
