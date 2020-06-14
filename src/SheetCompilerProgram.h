#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <compiler/ICompiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/IPreprocessor.h>
#include <fm/ILogger.h>
#include <ostream>


class SheetCompilerProgram {
private:
    std::shared_ptr<ICompilerProgramOptions> _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::MidiContextPtr      _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
public:
    SheetCompilerProgram(
        std::shared_ptr<ICompilerProgramOptions> programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::MidiContextPtr          context,
        sheet::compiler::IPreprocessorPtr        preprocessor
    ) : _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser),
        _compiler(compiler),
        _context(context),
        _preprocessor(preprocessor)
    {
    }
    int execute();
    void prepareEnvironment();
protected:
    virtual void prepareContext();
    void printIntro(std::ostream &os);
    void prepareSearchPaths();
    void prepareTemplateDefinitions();
private:
    void compile();
};

#endif
