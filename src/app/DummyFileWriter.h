#pragma once

#include "IDocumentWriter.h"
#include <forward.hpp>


namespace app
{
    class DummyFileWriter : public IDocumentWriter
    {
    public:
        virtual void write(documentModel::DocumentPtr) {}
        virtual void writeException(const std::exception &) {}
        virtual void writeException(const com::Exception &) {}
        virtual void writeUnknownException() {}
        virtual ~DummyFileWriter() = default;
    };
}