#ifndef FMAPP_JSONFILEWRITER_H
#define FMAPP_JSONFILEWRITER_H

#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include <fmapp/TimelineVisitor.hpp>
#include "JsonIOBase.h"
#include <compiler/IWarningsCollection.h>
#include "ADocumentWriter.h"
#include <ostream>

namespace fmapp {
    class JsonWriter : public JsonIOBase, public ADocumentWriter {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        fmapp::DefaultTimelinePtr  _timeline;
        fm::ILoggerPtr             _logger;
    public:
        typedef JsonIOBase Base;
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
        {
            initOutputStream();
        }
        void docToJson(std::ostream &, sheet::DocumentPtr document);
        void eventInfosToJson(std::ostream &os, sheet::DocumentPtr document);
        virtual void write(sheet::DocumentPtr document) override;
        void writeDocumentToJson(sheet::DocumentPtr document);
        void writeValidationJson(sheet::DocumentPtr document);
        virtual void writeException(const std::exception &ex) override;
        virtual void writeException(const fm::Exception &ex) override;
        virtual void writeUnknownException() override;        
        virtual ~JsonWriter() = default;
        void setOutputStream(std::ostream &os);
    protected:
        void initOutputStream();
        inline std::ostream & ostream() { return *_ostream; }
    private:
        sheet::compiler::IWarningsCollectionPtr getWarnings();
        std::ostream *_ostream = nullptr;
    };
}

#endif