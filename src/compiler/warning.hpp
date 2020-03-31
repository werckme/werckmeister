#ifndef COMPILER_WARNING_HPP
#define COMPILER_WARNING_HPP


#include <list>
#include <sheet/ASheetObject.hpp>
#include <string>
#include <memory>

namespace sheet {
    class Document;
    struct Warning
    {
        std::string message;
        ASheetObjectWithSourceInfo sourceObject;
        std::string toString(std::shared_ptr<Document> doc) const;
        std::string getSourceFile(std::shared_ptr<Document> doc) const;
    };
	typedef std::list<Warning> Warnings;
}

#endif