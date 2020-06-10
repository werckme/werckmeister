#include "SheetCompilerProgram.h"
#include <iostream>

SheetCompilerProgram::SheetCompilerProgram(sheet::compiler::IDocumentParser *documentParser)
    : _documentParser(documentParser)
{
    
}

void SheetCompilerProgram::execute()
{
    std::cout << "hello from sheet compiler program " << _documentParser << std::endl;
}