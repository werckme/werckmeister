#ifndef FMAPP_JSONWRITERBASE_H
#define FMAPP_JSONWRITERBASE_H

#include <forward.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <fmapp/TimelineVisitor.hpp>

namespace fmapp {
    class JsonWriterBase {
    private:
    public:
        void eventInfoToJSON(std::ostream& os, fm::Ticks elapsedTime, unsigned long lastUpdateTimestamp, const std::vector<fmapp::DefaultTimeline::EventInfo> &eventInfos, bool ignoreTimestamp = false);
        std::string base64Encode(const std::string &data);
        std::string base64Decode(const std::string &base64);
        std::string midiToBase64(fm::midi::MidiPtr midi);
        void toStream(std::ostream& os, rapidjson::Document &doc);
    };
}

#endif