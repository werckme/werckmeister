#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <compiler/ICompiler.h>
#include <fm/ILogger.h>
#include <ostream>

class SheetCompilerProgram {
private:
    fm::Werckmeister *_wm;
    std::shared_ptr<ICompilerProgramOptions> _programOptions;
    std::shared_ptr<fm::ILogger> _logger;
    sheet::compiler::IDocumentParser *_documentParser;
    sheet::compiler::ICompiler *_compiler;
public:
    SheetCompilerProgram(
        fm::Werckmeister *wm,
        std::shared_ptr<ICompilerProgramOptions> programOptions,
        std::shared_ptr<fm::ILogger> logger,
        sheet::compiler::IDocumentParser *documentParser,
        sheet::compiler::ICompiler *compiler
    ) : _wm(wm),
        _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser),
        _compiler(compiler)
    {
    }
    void execute();
    void prepareEnvironment();
protected:
    void printIntro(std::ostream &os);
    void prepareSearchPaths();
};

#endif
