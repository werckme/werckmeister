#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>

class SheetCompilerProgram {
public:
    SheetCompilerProgram(sheet::compiler::IDocumentParser *documentParser);
    void execute();
};

#endif
