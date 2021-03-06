#ifndef FMAPP_JSON_READER
#define FMAPP_JSON_READER

#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <vector>
#include <compiler/error.hpp>
#include <list>
#include <fmapp/JsonIOBase.h>

struct VirtualFileDto {
    std::string path;
    std::string data;
};

namespace fmapp {
    /**
     * creates a list of virtual files via a json string.
     * Needed if the compiler is in Json mode.
     */
    class JsonStringInputReader : public JsonIOBase {
    public:
        JsonStringInputReader() = default;
        JsonStringInputReader(const JsonStringInputReader&) = delete;
        JsonStringInputReader& operator= (const JsonStringInputReader&) = delete;
        std::list<VirtualFileDto> readVirtualFS(const std::string & jsonData);
    };
}

#endif