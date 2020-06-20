#ifndef FMAPP_IDOCUMENTWRITER_H
#define FMAPP_IDOCUMENTWRITER_H

#include <forward.hpp>
#include <exception>
#include <fm/exception.hpp>

namespace fmapp {
    class IDocumentWriter {
    public:
        virtual void write(sheet::DocumentPtr document) = 0;
        virtual void writeException(const std::exception &ex) = 0;
        virtual void writeException(const fm::Exception &ex) = 0;
        virtual void writeUnknownException() = 0;
    };
    typedef std::shared_ptr<IDocumentWriter> IDocumentWriterPtr; 
}

#endif