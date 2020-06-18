#include "SheetPlayerProgram.h"
#include <iostream>

void SheetPlayerProgram::onSheetChanged()
{
    _logger->babble(WMLogLambda(log << "sheet document changed"));
}