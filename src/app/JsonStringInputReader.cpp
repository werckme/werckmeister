#include "JsonStringInputReader.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <compiler/error.hpp>
#include <app/os.hpp>

namespace app {
    ///////////////////////////////////////////////////////////////////////////
    std::list<VirtualFileDto> JsonStringInputReader::readVirtualFS(const std::string & jsonData)
    {
        std::list<VirtualFileDto> result;
        rapidjson::Document document;
       document.Parse(jsonData.c_str());
        if (document.HasParseError()) {
            auto errOffset = document.GetErrorOffset(); 
            std::stringstream ss;
            ss << "failed parsing input json at offset: "  << errOffset;
            throw std::runtime_error(ss.str());
        }
        if (!document.IsArray()) {
            throw std::runtime_error("failed to load input json: expected an array");
        }
        for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
            if (!document[i].IsObject()) {
                std::stringstream ss;
                ss << "failed to load input json: expected an object at index: "  << i;
                throw std::runtime_error(ss.str());
            }
            const auto &obj = document[i].GetObject();
            VirtualFileDto dto;
            if (!obj.HasMember("path")) {
                std::stringstream ss;
                ss << "failed to load input json: expected a member \"path\" with object at index: "  << i;
                throw std::runtime_error(ss.str());
            }
            if (!obj.HasMember("data")) {
                std::stringstream ss;
                ss << "failed to load input json: expected a member \"data\" with object at index: "  << i;
                throw std::runtime_error(ss.str());
            }
            dto.path = obj["path"].GetString();
            dto.data = obj["data"].GetString();
            result.emplace_back(dto);
        }
        return result;
    }
}