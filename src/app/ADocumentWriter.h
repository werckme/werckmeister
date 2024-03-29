#pragma once

#include "IDocumentWriter.h"
#include <com/ILogger.h>

namespace app
{
    class ADocumentWriter : public IDocumentWriter
    {
    private:
        com::ILoggerPtr _logger;

    public:
        ADocumentWriter(com::ILoggerPtr logger) : _logger(logger) {}
        virtual void writeException(const std::exception &ex) override;
        virtual void writeException(const com::Exception &ex) override;
        virtual void writeUnknownException() override;
    };
}
