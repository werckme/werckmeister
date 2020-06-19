#include "SheetPlayerProgram.h"
#include <iostream>

void SheetPlayerProgram::onSheetChanged()
{
    _logger->babble(WMLogLambda(log << "sheet document changed"));
    auto ticksStopped = _midiPlayerPtr->stop();
    _programOptions->setBegin(ticksStopped / fm::PPQ );
    documentWasChanged = true;
    _logger->babble(WMLogLambda(log << "resume at tick: " << ticksStopped));
}

int SheetPlayerProgram::execute()
{
    int result = Base::execute();
    if (result != 0) {
        return result;
    }
    if (documentWasChanged) {
        return RetCodeRestart;
    }
    return 0;
}