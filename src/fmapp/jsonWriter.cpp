#include "jsonWriter.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <sheet/Document.h>
#include <fmapp/MidiAndTimeline.hpp>
#include <compiler/error.hpp>

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
    std::string JsonWriter::funkfeuerToJSON(fm::Ticks elapsedTime, unsigned long lastUpdateTimestamp)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        return toString(doc);
    }

    std::string JsonWriter::funkfeuerToJSON(fm::Ticks elapsedTime, unsigned long lastUpdateTimestamp, const std::vector<EventInfo> &eventInfos)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        rapidjson::Value lastUpdate;
		lastUpdate.SetUint(lastUpdateTimestamp);
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        doc.AddMember("lastUpdateTimestamp", lastUpdate, doc.GetAllocator());
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

    std::string JsonWriter::documentInfosToJSON(const sheet::Document &document, fm::Ticks duration, const sheet::Warnings &warnings)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value warningsArray(rapidjson::kArrayType);
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
        for (const auto &warning : warnings) {
            rapidjson::Value object(rapidjson::kObjectType);
            rapidjson::Value message;
            message.SetString(warning.c_str(), doc.GetAllocator());
            object.AddMember("message", message, doc.GetAllocator());
            warningsArray.PushBack(object, doc.GetAllocator());
        }
        doc.AddMember("sources", array, doc.GetAllocator());
        doc.AddMember("duration", durationValue, doc.GetAllocator());
        doc.AddMember("warnings", warningsArray, doc.GetAllocator());
        return toString(doc);
    }

    std::string JsonWriter::exceptionToJSON(const std::exception &ex)
    {
        typedef sheet::compiler::Exception SheetException;
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value errorMessage;
        const SheetException *fmex = dynamic_cast<const SheetException*>(&ex);
        const sheet::ASheetObjectWithSourceInfo *docInfo = fmex ? fmex->getSourceInfo() : nullptr;
        if (fmex && docInfo) {
            rapidjson::Value sourceId(docInfo->sourceId);
            rapidjson::Value positionBegin(docInfo->sourcePositionBegin);
            rapidjson::Value positionEnd(docInfo->sourcePositionEnd);
            errorMessage.SetString(ex.what(), doc.GetAllocator());
            doc.AddMember("sourceId", sourceId, doc.GetAllocator());
            if (docInfo->sourcePositionBegin != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
                doc.AddMember("positionBegin", positionBegin, doc.GetAllocator());
            }
            if (docInfo->sourcePositionEnd != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
                doc.AddMember("positionEnd", positionEnd, doc.GetAllocator());
            }
        } else {
            errorMessage.SetString(ex.what(), doc.GetAllocator());
        }
        doc.AddMember("errorMessage", errorMessage, doc.GetAllocator());
        return toString(doc);
    }
}