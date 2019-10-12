#include "jsonWriter.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <sheet/Document.h>

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
        rapidjson::Value type("funkfeuer");
        doc.AddMember("type", type, doc.GetAllocator());
        return toString(doc);
    }

    std::string JsonWriter::documentInfosToJSON(const sheet::Document &document)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value type("documentInfos");
        doc.AddMember("type", type, doc.GetAllocator());
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
        return toString(doc);
    }
}