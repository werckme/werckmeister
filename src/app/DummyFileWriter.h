#pragma once

#include "IDocumentWriter.h"
#include <forward.hpp>


namespace app
{
    class DummyFileWriter : public IDocumentWriter
    {
    public:
        virtual void write(documentModel::DocumentPtr document) {}
        virtual void writeException(const std::exception &ex) {}
        virtual void writeException(const com::Exception &ex) {}
        virtual void writeUnknownException() {}
        virtual ~DummyFileWriter() = default;
    };
}