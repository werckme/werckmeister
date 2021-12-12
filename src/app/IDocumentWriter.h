#pragma once

#include <forward.hpp>
#include <exception>
#include <com/exception.hpp>

namespace app
{
    class IDocumentWriter
    {
    public:
        virtual void write(documentModel::DocumentPtr document) = 0;
        virtual void writeException(const std::exception &ex) = 0;
        virtual void writeException(const com::Exception &ex) = 0;
        virtual void writeUnknownException() = 0;
    };
    typedef std::shared_ptr<IDocumentWriter> IDocumentWriterPtr;
}
