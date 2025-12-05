#include "SheetLibProgram.h"


int SheetLibProgram::execute()
{
    if (!_programOptions->isInputSet())
    {
        throw std::runtime_error("missing input file");
    }
    compile();
    return 0;
}