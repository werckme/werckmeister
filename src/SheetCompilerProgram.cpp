#include "SheetCompilerProgram.h"
#include <iostream>

SheetCompilerProgram::SheetCompilerProgram(sheet::compiler::IDocumentParser *documentParser)
{
    
}

void SheetCompilerProgram::execute()
{
    std::cout << "hello from sheet compiler program" << std::endl;
}