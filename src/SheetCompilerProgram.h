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
#include <fmapp/IDocumentWriter.h>

class SheetCompilerProgram {
private:
    ICompilerProgramOptionsPtr _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::MidiContextPtr      _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
    fm::midi::MidiPtr                    _midiFile;
    fmapp::IDocumentWriterPtr            _documentWriter;

public:
    SheetCompilerProgram(
        ICompilerProgramOptionsPtr               programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::MidiContextPtr          context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::IDocumentWriterPtr                documentWriter
    ) : _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser),
        _compiler(compiler),
        _context(context),
        _preprocessor(preprocessor),
        _midiFile(midiFile),
        _documentWriter(documentWriter)
    {
    }
    int execute();
    void prepareEnvironment();
    virtual ~SheetCompilerProgram() = default;
protected:
    virtual void prepareContext();
    void printIntro(std::ostream &os);
    void prepareSearchPaths();
    void prepareTemplateDefinitions();
    void addSearchPath(const fm::String &path);
private:
    void compile();
};

#endif
