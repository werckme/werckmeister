#ifndef FMAPP_JSON_WRITER
#define FMAPP_JSON_WRITER

#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>

namespace fmapp {

    /**
     * writes sheet info as json format
     **/
    class JsonWriter {
        public:
            JsonWriter() = default;
            JsonWriter(const JsonWriter&) = delete;
            JsonWriter& operator= (const JsonWriter&) = delete;
            std::string write(fm::Ticks elapsedTime);
    };
}

#endif