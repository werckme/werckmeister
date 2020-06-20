#ifndef FMAPP_JSONFILEWRITER_H
#define FMAPP_JSONFILEWRITER_H

#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include <fmapp/TimelineVisitor.hpp>
#include "JsonWriterBase.h"
#include <compiler/IWarningsCollection.h>
#include "ADocumentWriter.h"

namespace fmapp {
    class JsonWriter : public JsonWriterBase, public ADocumentWriter {
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
            : ADocumentWriter(logger),
              _programOptions(programOptions), 
              _midifile      (midiFile),
              _timeline      (timeline),
              _logger        (logger)
        {}
        void docToJson(std::ostream &, sheet::DocumentPtr document);
        void eventInfosToJson(std::ostream &os, sheet::DocumentPtr document);
        virtual void write(sheet::DocumentPtr document) override;
        void writeDocumentToJson(sheet::DocumentPtr document);
        void writeValidationJson(sheet::DocumentPtr document);
        virtual void writeException(const std::exception &ex) override;
        virtual void writeException(const fm::Exception &ex) override;
        virtual void writeUnknownException() override;        
        virtual ~JsonWriter() = default;
    private:
        sheet::compiler::IWarningsCollectionPtr getWarnings();
    };
}

#endif