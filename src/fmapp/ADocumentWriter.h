#ifndef FMAPP_ADOCUMENTWRITER_H
#define FMAPP_ADOCUMENTWRITER_H

#include "IDocumentWriter.h"
#include <fm/ILogger.h>

namespace fmapp {
    class ADocumentWriter : public IDocumentWriter {
    private:
        fm::ILoggerPtr _logger;
    public:
        ADocumentWriter(fm::ILoggerPtr logger) : _logger(logger) {}
        virtual void writeException(const std::exception &ex) override;
        virtual void writeException(const fm::Exception &ex) override;
        virtual void writeUnknownException() override;
    };
}

#endif