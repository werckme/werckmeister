#include "JsonWriter.h"
#include <iostream>
#include <documentModel/Document.h>
#include <com/config/configServer.h>
#include <com/config.hpp>
namespace
{
    rapidjson::Document documentInfosToJSONDoc(documentModel::DocumentPtr sheetDoc, com::Ticks duration, const compiler::Warnings &warnings)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value warningsArray(rapidjson::kArrayType);
        rapidjson::Value durationValue((double)duration);
        for (const auto &source : sheetDoc->sources.left)
        {
            rapidjson::Value object(rapidjson::kObjectType);
            rapidjson::Value sourceId(source.first);
            object.AddMember("sourceId", sourceId, doc.GetAllocator());
            rapidjson::Value path;
            path.SetString(rapidjson::StringRef(source.second.c_str()));
            object.AddMember("path", path, doc.GetAllocator());
            array.PushBack(object, doc.GetAllocator());
        }
        for (const auto &warning : warnings)
        {
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
#if IS_EMSCRIPTEN_BUILD == 1
        rapidjson::Value devicedArray(rapidjson::kArrayType);
        for(const auto& device : com::getConfigServer().getDevices())
        {
            if (device.second.type != com::DeviceConfig::WebPlayer) {
                continue;
            }
            rapidjson::Value object(rapidjson::kObjectType);
            rapidjson::Value name;
            rapidjson::Value fontName;
            name.SetString(device.second.name.c_str(), doc.GetAllocator());
            fontName.SetString(device.second.deviceId.c_str(), doc.GetAllocator());
            object.AddMember("name", name, doc.GetAllocator());
            object.AddMember("fontName", fontName, doc.GetAllocator());
            devicedArray.PushBack(object, doc.GetAllocator());
        }
        doc.AddMember("devices", devicedArray, doc.GetAllocator());
#endif
        return doc;
    }
}

namespace app
{

    void JsonWriter::initOutputStream()
    {
        this->_ostream = &std::cout;
    }

    void JsonWriter::setOutputStream(std::ostream &os)
    {
        this->_ostream = &os;
    }

    void JsonWriter::write(documentModel::DocumentPtr document)
    {
        if (_programOptions->isJsonModeSet())
        {
            writeDocumentToJson(document);
        }
        if (_programOptions->isJsonDocInfoMode())
        {
            writeValidationJson(document);
        }
        if (_programOptions->isJsonDebugInfoMode())
        {
            writeDebugInfos();
        }
    }

    void JsonWriter::writeException(const std::exception &ex)
    {
        exceptionToJSON(ostream(), ex);
    }

    void JsonWriter::writeException(const com::Exception &ex)
    {
        exceptionToJSON(ostream(), ex);
    }

    void JsonWriter::writeUnknownException()
    {
        std::runtime_error ex("unkown error");
        exceptionToJSON(ostream(), ex);
    }

    compiler::IWarningsCollectionPtr JsonWriter::getWarnings()
    {
        auto waningsCollection = std::dynamic_pointer_cast<compiler::IWarningsCollection>(_logger);
        return waningsCollection;
    }

    void JsonWriter::writeValidationJson(documentModel::DocumentPtr document)
    {
        compiler::Warnings __warnings;
        const compiler::Warnings *warnings = &__warnings;
        auto waningsCollection = getWarnings();
        if (waningsCollection)
        {
            warnings = &(waningsCollection->warnings());
        }
        rapidjson::Document doc = documentInfosToJSONDoc(document, _midifile->duration(), *warnings);
        toStream(ostream(), doc);
    }
    void JsonWriter::writeDocumentToJson(documentModel::DocumentPtr document)
    {
        ostream()
            << "{"
            << "\"midi\": ";
        docToJson(ostream(), document);
        ostream() << ", \"eventInfos\": ";
        eventInfosToJson(ostream(), document);
        ostream() << "}" <<std::endl;
    }
    void JsonWriter::docToJson(std::ostream &os, documentModel::DocumentPtr document)
    {
        compiler::Warnings __warnings;
        const compiler::Warnings *warnings = &__warnings;
        auto waningsCollection = getWarnings();
        if (waningsCollection)
        {
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

    void JsonWriter::eventInfosToJson(std::ostream &os, documentModel::DocumentPtr document)
    {
        os << "[" << std::endl;

        bool first = true;
        for (const auto &timelineEntry : _timeline->intervalContainer())
        {
            app::EventInfos eventInfos;
            com::Ticks eventsBeginTime = timelineEntry.first.lower() / (double)com::PPQ;
            eventInfos.reserve(timelineEntry.second.size());
            for (const auto &x : timelineEntry.second)
            {
                eventInfos.push_back(x);
            }
            if (!first)
            {
                os << ", ";
            }
            eventInfoToJSON(os, eventsBeginTime, 0, eventInfos, true);
            first = false;
        }
        os << "]";
    }

    void JsonWriter::writeDebugInfos()
    {
        rapidjson::Document doc;
        doc.SetObject();
        toStream(ostream(), doc);
    }
}