#pragma once

#include <list>
#include <documentModel/objects/ASheetObjectWithSourceInfo.h>
#include <string>
#include <memory>

namespace documentModel
{
    class Document;
}
namespace compiler
{
    struct Warning
    {
        std::string message;
        ASheetObjectWithSourceInfo sourceObject;
        std::string toString(std::shared_ptr<Document> doc) const;
        std::string getSourceFile(std::shared_ptr<Document> doc) const;
    };
    typedef std::list<Warning> Warnings;
}
