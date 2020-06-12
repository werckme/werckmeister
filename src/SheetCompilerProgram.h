#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <compiler/ICompiler.h>
#include <compiler/context/IContext.h>
#include <fm/ILogger.h>
#include <ostream>


class SheetCompilerProgram {
private:
    fm::Werckmeister *_wm;
    std::shared_ptr<ICompilerProgramOptions> _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParser *_documentParser;
    sheet::compiler::ICompiler *_compiler;
    sheet::compiler::IContext *_context;
public:
    SheetCompilerProgram(
        fm::Werckmeister *wm,
        std::shared_ptr<ICompilerProgramOptions> programOptions,
        fm::ILoggerPtr logger,
        sheet::compiler::IDocumentParser *documentParser,
        sheet::compiler::ICompiler *compiler,
        sheet::compiler::IContext *context
    ) : _wm(wm),
        _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser),
        _compiler(compiler),
        _context(context)
    {
    }
    void execute();
    void prepareEnvironment();
protected:
    void printIntro(std::ostream &os);
    void prepareSearchPaths();
};

#endif
