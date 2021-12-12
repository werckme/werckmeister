#pragma once

#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <com/ILogger.h>
#include <app/TimelineVisitor.hpp>
#include "JsonIOBase.h"
#include <compiler/IWarningsCollection.h>
#include "ADocumentWriter.h"
#include <ostream>

namespace app
{
    class JsonWriter : public JsonIOBase, public ADocumentWriter
    {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        com::midi::MidiPtr _midifile;
        app::DefaultTimelinePtr _timeline;
        com::ILoggerPtr _logger;

    public:
        typedef JsonIOBase Base;
        JsonWriter(
            ICompilerProgramOptionsPtr programOptions,
            com::midi::MidiPtr midiFile,
            app::DefaultTimelinePtr timeline,
            com::ILoggerPtr logger)
            : ADocumentWriter(logger),
              _programOptions(programOptions),
              _midifile(midiFile),
              _timeline(timeline),
              _logger(logger)
        {
            initOutputStream();
        }
        void docToJson(std::ostream &, documentModel::DocumentPtr document);
        void eventInfosToJson(std::ostream &os, documentModel::DocumentPtr document);
        virtual void write(documentModel::DocumentPtr document) override;
        void writeDocumentToJson(documentModel::DocumentPtr document);
        void writeValidationJson(documentModel::DocumentPtr document);
        virtual void writeException(const std::exception &ex) override;
        virtual void writeException(const com::Exception &ex) override;
        virtual void writeUnknownException() override;
        virtual ~JsonWriter() = default;
        void setOutputStream(std::ostream &os);

    protected:
        void initOutputStream();
        inline std::ostream &ostream() { return *_ostream; }

    private:
        compiler::IWarningsCollectionPtr getWarnings();
        std::ostream *_ostream = nullptr;
    };
}
