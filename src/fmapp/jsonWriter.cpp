#include "jsonWriter.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

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
    std::string JsonWriter::write(fm::Ticks elapsedTime)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        return toString(doc);
    }
}