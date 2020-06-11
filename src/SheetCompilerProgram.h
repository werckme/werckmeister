#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <fm/ILogger.h>
#include <ostream>

class SheetCompilerProgram {
private:
    fm::Werckmeister *_wm;
    std::shared_ptr<ICompilerProgramOptions> _programOptions;
    std::shared_ptr<fm::ILogger> _logger;
    sheet::compiler::IDocumentParser *_documentParser;
public:
    SheetCompilerProgram(
        fm::Werckmeister *wm,
        std::shared_ptr<ICompilerProgramOptions> programOptions,
        std::shared_ptr<fm::ILogger> logger,
        sheet::compiler::IDocumentParser *documentParser
    ) : _wm(wm),
        _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser)
    {
    }
    void execute();
    void prepareEnvironment();
protected:
    void printIntro(std::ostream &os);
    void prepareSearchPaths();
};

#endif
