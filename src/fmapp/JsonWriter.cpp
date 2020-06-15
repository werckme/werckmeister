#include "JsonWriter.h"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <fm/midi.hpp>
#include <iostream>
#include <sheet/Document.h>
#include <compiler/error.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <fm/midi.hpp>
#include <fmapp/os.hpp>

namespace {
    void toStream(std::ostream& os, rapidjson::Document &doc) 
    {
        rapidjson::OStreamWrapper osw(os);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        doc.Accept(writer);
    }    
    rapidjson::Document documentInfosToJSONDoc(sheet::DocumentPtr sheetDoc, fm::Ticks duration)
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
        // TODO: #126
        // for (const auto &warning : warnings) {
        //     rapidjson::Value object(rapidjson::kObjectType);
        //     rapidjson::Value message;
        //     rapidjson::Value path;
        //     path.SetString(warning.getSourceFile(sheetDoc).c_str(), doc.GetAllocator());
        //     rapidjson::Value sourceId(warning.sourceObject.sourceId);
        //     rapidjson::Value positionBegin(warning.sourceObject.sourcePositionBegin);
        //     message.SetString(warning.message.c_str(), doc.GetAllocator());
        //     object.AddMember("message", message, doc.GetAllocator());
        //     object.AddMember("sourceFile", path, doc.GetAllocator());
        //     object.AddMember("sourceId", sourceId, doc.GetAllocator());
        //     object.AddMember("positionBegin", positionBegin, doc.GetAllocator());
        //     warningsArray.PushBack(object, doc.GetAllocator());
        // }
        doc.AddMember("sources", array, doc.GetAllocator());
        doc.AddMember("duration", durationValue, doc.GetAllocator());
        doc.AddMember("warnings", warningsArray, doc.GetAllocator());
        return doc;
    }
    void eventInfoToJSON(std::ostream& os,
        fm::Ticks elapsedTime, 
        unsigned long lastUpdateTimestamp, 
        const std::vector<fmapp::DefaultTimeline::EventInfo> &eventInfos, 
        bool ignoreTimestamp)
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value elapsed;
        elapsed.SetDouble(elapsedTime);
        rapidjson::Value lastUpdate;
		lastUpdate.SetUint(lastUpdateTimestamp);
        rapidjson::Value pid;
        pid.SetInt(fmapp::os::getPId());
        doc.AddMember("sheetTime", elapsed, doc.GetAllocator());
        doc.AddMember("pid", pid, doc.GetAllocator());
        if (!ignoreTimestamp) {
            doc.AddMember("lastUpdateTimestamp", lastUpdate, doc.GetAllocator());
        }
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
        toStream(os, doc);
    }
}


namespace fmapp {
    void JsonWriter::write(sheet::DocumentPtr document)
    {
        std::cout 
        << "{" 
        << "\"midi\": ";
        docToJson(std::cout, document);
        std::cout << ", \"eventInfos\": ";
        eventInfosAsJson(std::cout, document);
        std::cout << "}" 
        << std::endl
        ;
    }
    void JsonWriter::docToJson(std::ostream& os, sheet::DocumentPtr document)
    {
        rapidjson::Document doc = documentInfosToJSONDoc(document, _midifile->duration());
        rapidjson::Value midiData;
        rapidjson::Value bpm(_midifile->bpm());
        midiData.SetString(midiToBase64(_midifile).c_str(), doc.GetAllocator());
        doc.AddMember("midiData", midiData, doc.GetAllocator());
        doc.AddMember("bpm", bpm, doc.GetAllocator());
        toStream(os, doc);
    }
    void JsonWriter::eventInfosAsJson(std::ostream &os, sheet::DocumentPtr document)
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

    std::string JsonWriter::midiToBase64(fm::midi::MidiPtr midi)
    {
        size_t nbytes = midi->byteSize();
        size_t nBase64 = boost::beast::detail::base64::encoded_size(nbytes);
        fm::Byte *bff = new fm::Byte[nbytes];
        char *base64 = new char[nBase64];
        midi->write(bff, nbytes);
        auto writtenBytes = boost::beast::detail::base64::encode(base64, bff, nbytes);
        std::string result(base64, writtenBytes);
        delete []bff;
        delete []base64;
        return result;
    }

    std::string JsonWriter::base64Encode(const std::string &data)
    {
        size_t nbytes = data.size();
        size_t nBase64 = boost::beast::detail::base64::encoded_size(nbytes);
        char *base64 = new char[nBase64];
        auto writtenBytes = boost::beast::detail::base64::encode(base64, data.c_str(), nbytes);
        std::string result(base64, writtenBytes);
        delete []base64;
        return result;
    }
    std::string JsonWriter::base64Decode(const std::string &base64)
    {
        size_t nBase64 = base64.size();
        size_t nbytes = boost::beast::detail::base64::decoded_size(nBase64);
        char *data = new char[nbytes];
        auto writtenBytes = boost::beast::detail::base64::decode(data, base64.c_str(), nBase64);
        std::string result(data, writtenBytes.first);
        delete []data;
        return result;
    }
}