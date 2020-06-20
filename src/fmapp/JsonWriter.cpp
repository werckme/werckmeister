#include "JsonWriter.h"
#include <iostream>
#include <sheet/Document.h>

namespace {
    rapidjson::Document documentInfosToJSONDoc(sheet::DocumentPtr sheetDoc, fm::Ticks duration, const sheet::Warnings &warnings)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value warningsArray(rapidjson::kArrayType);
        rapidjson::Value durationValue((double)duration);
        for (const auto &source : sheetDoc->sources.left) {
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
            rapidjson::Value path;
            path.SetString(warning.getSourceFile(sheetDoc).c_str(), doc.GetAllocator());
            rapidjson::Value sourceId(warning.sourceObject.sourceId);
            rapidjson::Value positionBegin(warning.sourceObject.sourcePositionBegin);
            message.SetString(warning.message.c_str(), doc.GetAllocator());
            object.AddMember("message", message, doc.GetAllocator());
            object.AddMember("sourceFile", path, doc.GetAllocator());
            object.AddMember("sourceId", sourceId, doc.GetAllocator());
            object.AddMember("positionBegin", positionBegin, doc.GetAllocator());
            warningsArray.PushBack(object, doc.GetAllocator());
        }
        doc.AddMember("sources", array, doc.GetAllocator());
        doc.AddMember("duration", durationValue, doc.GetAllocator());
        doc.AddMember("warnings", warningsArray, doc.GetAllocator());
        return doc;
    }
}

namespace fmapp {
    void JsonWriter::write(sheet::DocumentPtr document)
    {
        if (_programOptions->isJsonModeSet()) {
            writeDocumentToJson(document);
        }
        if (_programOptions->isJsonDocInfoMode()) {
            writeValidationJson(document);
        }
    }

    void JsonWriter::writeException(const std::exception &ex)
    {
        exceptionToJSON(std::cout, ex);
    }

    void JsonWriter::writeException(const fm::Exception &ex)
    {
        exceptionToJSON(std::cout, ex);
    }

    void JsonWriter::writeUnknownException()
    {
        std::runtime_error ex("unkown error");
        exceptionToJSON(std::cout, ex);
    }

    sheet::compiler::IWarningsCollectionPtr JsonWriter::getWarnings()
    {
        auto waningsCollection = std::dynamic_pointer_cast<sheet::compiler::IWarningsCollection>(_logger);
        return waningsCollection;
    }

    void JsonWriter::writeValidationJson(sheet::DocumentPtr document)
    {
        sheet::Warnings __warnings;
        const sheet::Warnings *warnings = &__warnings;
        auto waningsCollection = getWarnings();
        if (waningsCollection) {
            warnings = &(waningsCollection->warnings());
        }
        rapidjson::Document doc = documentInfosToJSONDoc(document, _midifile->duration(), *warnings);
        toStream(std::cout, doc);
    }
    void JsonWriter::writeDocumentToJson(sheet::DocumentPtr document)
    {
        std::cout 
        << "{" 
        << "\"midi\": ";
        docToJson(std::cout, document);
        std::cout << ", \"eventInfos\": ";
        eventInfosToJson(std::cout, document);
        std::cout << "}" 
        << std::endl
        ;
    }
    void JsonWriter::docToJson(std::ostream& os, sheet::DocumentPtr document)
    {
        sheet::Warnings __warnings;
        const sheet::Warnings *warnings = &__warnings;
        auto waningsCollection = getWarnings();
        if (waningsCollection) {
            warnings = &(waningsCollection->warnings());
        }
        rapidjson::Document doc = documentInfosToJSONDoc(document, _midifile->duration(), *warnings);
        rapidjson::Value midiData;
        rapidjson::Value bpm(_midifile->bpm());
        midiData.SetString(midiToBase64(_midifile).c_str(), doc.GetAllocator());
        doc.AddMember("midiData", midiData, doc.GetAllocator());
        doc.AddMember("bpm", bpm, doc.GetAllocator());
        toStream(os, doc);
    }

    void JsonWriter::eventInfosToJson(std::ostream &os, sheet::DocumentPtr document)
    {
        os << "[" << std::endl;
        
        bool first = true;
        for (const auto &timelineEntry : _timeline->intervalContainer()) {
            fmapp::EventInfos eventInfos;
            fm::Ticks eventsBeginTime = timelineEntry.first.lower() / (double)fm::PPQ;
            eventInfos.reserve(timelineEntry.second.size());
            for (const auto &x : timelineEntry.second) {
                eventInfos.push_back(x);
            }
            if (!first) {
                os << ", ";
            }
            eventInfoToJSON(os, eventsBeginTime, 0, eventInfos, true); 
            first = false;
        }
        os << "]";
    }
}