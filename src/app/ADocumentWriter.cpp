#include "ADocumentWriter.h"

namespace app
{
    void ADocumentWriter::writeException(const std::exception &ex)
    {
        _logger->error(WMLogLambda(log << ex.what()));
    }
    void ADocumentWriter::writeException(const com::Exception &ex)
    {
        _logger->error(WMLogLambda(log << ex.toString()));
    }
    void ADocumentWriter::writeUnknownException()
    {
        _logger->error(WMLogLambda(log << "unkown error"));
    }
}