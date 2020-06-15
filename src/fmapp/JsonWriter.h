#ifndef FMAPP_JSONFILEWRITER_H
#define FMAPP_JSONFILEWRITER_H

#include "IDocumentWriter.h"
#include <forward.hpp>
#include <ICompilerProgramOptions.h>
#include <fm/ILogger.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <fmapp/TimelineVisitor.hpp>

namespace fmapp {
    class JsonWriter : public IDocumentWriter {
    private:
        ICompilerProgramOptionsPtr _programOptions;
        fm::midi::MidiPtr          _midifile;
        fmapp::DefaultTimelinePtr  _timeline;
        fm::ILoggerPtr             _logger;
    public:
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
        virtual void write(sheet::DocumentPtr document);
        void docToJson(std::ostream &, sheet::DocumentPtr document);
        void eventInfosAsJson(std::ostream &, sheet::DocumentPtr document);
        std::string base64Encode(const std::string &data);
        std::string base64Decode(const std::string &base64);
        std::string midiToBase64(fm::midi::MidiPtr midi);
        virtual ~JsonWriter() = default;
    };
}

#endif