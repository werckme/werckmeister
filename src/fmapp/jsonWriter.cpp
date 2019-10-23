#include "jsonWriter.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <sheet/Document.h>
#include <fmapp/MidiAndTimeline.hpp>

namespace {
    std::string toString(const rapidjson::Document &doc) 
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }
}


namespace fmapp {
    std::string JsonWriter::funkfeuerToJSON(fm::Ticks elapsedTime)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        return toString(doc);
    }

    std::string JsonWriter::funkfeuerToJSON(fm::Ticks elapsedTime, const std::vector<EventInfo> &eventInfos)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        rapidjson::Value array(rapidjson::kArrayType);
        for (const auto &eventInfo : eventInfos) {
            rapidjson::Value object(rapidjson::kObjectType);
            rapidjson::Value beginPosition(eventInfo.beginPosition);
            object.AddMember("beginPosition", beginPosition, doc.GetAllocator());
            if (eventInfo.endPosition > 0) {
                rapidjson::Value endPosition(eventInfo.endPosition);
                object.AddMember("endPosition", endPosition, doc.GetAllocator());
            }
            rapidjson::Value sourceId(eventInfo.sourceId);
            object.AddMember("sourceId", sourceId, doc.GetAllocator());
            array.PushBack(object, doc.GetAllocator());
        }
        doc.AddMember("sheetEventInfos", array, doc.GetAllocator());
        return toString(doc);
    }

    std::string JsonWriter::documentInfosToJSON(const sheet::Document &document, fm::Ticks duration)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value durationValue((double)duration);
        for (const auto &source : document.sources.left) {
            rapidjson::Value object(rapidjson::kObjectType);
            rapidjson::Value sourceId(source.first);
            object.AddMember("sourceId", sourceId, doc.GetAllocator());
            rapidjson::Value path;
            path.SetString(rapidjson::StringRef(source.second.c_str()));
            object.AddMember("path", path, doc.GetAllocator());
            array.PushBack(object, doc.GetAllocator());
	    }
        doc.AddMember("sources", array, doc.GetAllocator());
        doc.AddMember("duration", durationValue, doc.GetAllocator());
        return toString(doc);
    }
}