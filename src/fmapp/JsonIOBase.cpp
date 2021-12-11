#include "JsonIOBase.h"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <com/midi.hpp>
#include <iostream>
#include <sheet/Document.h>
#include <compiler/error.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <com/midi.hpp>
#include <fmapp/os.hpp>


namespace {
    void toStream(std::ostream& os, rapidjson::Document &doc) 
    {
        rapidjson::OStreamWrapper osw(os);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        doc.Accept(writer);
        os << std::endl;
    }    
}


namespace fmapp {

    void JsonIOBase::toStream(std::ostream& os, rapidjson::Document &doc) 
    {
        return ::toStream(os, doc);
    }

    std::string JsonIOBase::midiToBase64(com::midi::MidiPtr midi)
    {
        size_t nbytes = midi->byteSize();
        size_t nBase64 = boost::beast::detail::base64::encoded_size(nbytes);
        com::Byte *bff = new com::Byte[nbytes];
        char *base64 = new char[nBase64];
        midi->write(bff, nbytes);
        auto writtenBytes = boost::beast::detail::base64::encode(base64, bff, nbytes);
        std::string result(base64, writtenBytes);
        delete []bff;
        delete []base64;
        return result;
    }

    std::string JsonIOBase::base64Encode(const std::string &data)
    {
        size_t nbytes = data.size();
        size_t nBase64 = boost::beast::detail::base64::encoded_size(nbytes);
        char *base64 = new char[nBase64];
        auto writtenBytes = boost::beast::detail::base64::encode(base64, data.c_str(), nbytes);
        std::string result(base64, writtenBytes);
        delete []base64;
        return result;
    }
    std::string JsonIOBase::base64Decode(const std::string &base64)
    {
        size_t nBase64 = base64.size();
        size_t nbytes = boost::beast::detail::base64::decoded_size(nBase64);
        char *data = new char[nbytes];
        auto writtenBytes = boost::beast::detail::base64::decode(data, base64.c_str(), nBase64);
        std::string result(data, writtenBytes.first);
        delete []data;
        return result;
    }

    void JsonIOBase::eventInfoToJSON(std::ostream& os,
        com::Ticks elapsedTime, 
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

    void JsonIOBase::exceptionToJSON(std::ostream& os,const std::exception &ex)
    {
        typedef sheet::compiler::Exception SheetException;
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value errorMessage;
        const SheetException *fmex                       = dynamic_cast<const SheetException*>(&ex);
        const sheet::ASheetObjectWithSourceInfo *docInfo = fmex ? fmex->getSourceInfo() : nullptr;
        if (fmex && docInfo) {
            const auto sourceFile = fmex->getSourceFile(); 
            rapidjson::Value sourceId(docInfo->sourceId);
            rapidjson::Value positionBegin(docInfo->sourcePositionBegin);
            rapidjson::Value positionEnd(docInfo->sourcePositionEnd);
            errorMessage.SetString(fmex->what(), doc.GetAllocator());
            doc.AddMember("sourceId", sourceId, doc.GetAllocator());
            if (docInfo->sourcePositionBegin != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
                doc.AddMember("positionBegin", positionBegin, doc.GetAllocator());
            }
            if (docInfo->sourcePositionEnd != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
                doc.AddMember("positionEnd", positionEnd, doc.GetAllocator());
            }
            if (true) {
                rapidjson::Value jsonSourceFile;
                jsonSourceFile.SetString(sourceFile.c_str(), doc.GetAllocator());
                doc.AddMember("sourceFile", jsonSourceFile, doc.GetAllocator());
            }
        } else {
            errorMessage.SetString(ex.what(), doc.GetAllocator());
        }
        doc.AddMember("errorMessage", errorMessage, doc.GetAllocator());
        toStream(os, doc);
    }
}