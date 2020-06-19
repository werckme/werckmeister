#include "SheetPlayerProgram.h"
#include <iostream>

void SheetPlayerProgram::onSheetChanged()
{
    _logger->babble(WMLogLambda(log << "sheet document changed"));
    _midiPlayerPtr->pause();
    _midiFile->clear();
    _context->clear();
    // TODO: #126 timeline.clear();
    compile();
    //_midiPlayerPtr->resume();
}