#ifndef FMAPP_JSONFILEWRITER_H
#define FMAPP_JSONFILEWRITER_H

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include <fmapp/TimelineVisitor.hpp>
#include "JsonWriterBase.h"

namespace fmapp {
    class JsonWriter : public JsonWriterBase, public IDocumentWriter {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        fmapp::DefaultTimelinePtr  _timeline;
        fm::ILoggerPtr             _logger;
    public:
        typedef JsonWriterBase Base;
        JsonWriter(
            ICompilerProgramOptionsPtr  programOptions, 
            fm::midi::MidiPtr           midiFile,
            fmapp::DefaultTimelinePtr   timeline,
            fm::ILoggerPtr              logger) 
            : _programOptions(programOptions), 
              _midifile      (midiFile),
              _timeline      (timeline),
              _logger        (logger)
        {}
        void docToJson(std::ostream &, sheet::DocumentPtr document);
        void eventInfosToJson(std::ostream &os, sheet::DocumentPtr document);
        virtual void write(sheet::DocumentPtr document);
        virtual ~JsonWriter() = default;
    };
}

#endif