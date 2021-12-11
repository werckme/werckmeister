#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <com/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <compiler/ICompiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/IPreprocessor.h>
#include <conductor/IConductionsPerformer.h>
#include <com/ILogger.h>
#include <ostream>
#include <app/IDocumentWriter.h>

class SheetCompilerProgram {
protected:
    ICompilerProgramOptionsPtr _programOptions;
    com::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::IContextPtr         _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
    com::midi::MidiPtr                    _midiFile;
    app::IDocumentWriterPtr            _documentWriter;
    sheet::conductor::IConductionsPerformerPtr      _conductionsPerformer;

public:
    SheetCompilerProgram(
        ICompilerProgramOptionsPtr               programOptions,
        com::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::IContextPtr             context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        com::midi::MidiPtr                        midiFile,
        app::IDocumentWriterPtr                documentWriter,
        sheet::conductor::IConductionsPerformerPtr          conductionsPerformer
    ) : _programOptions(programOptions),
        _logger(logger),
        _documentParser(documentParser),
        _compiler(compiler),
        _context(context),
        _preprocessor(preprocessor),
        _midiFile(midiFile),
        _documentWriter(documentWriter),
        _conductionsPerformer(conductionsPerformer)
    {
    }
    virtual int execute();
    virtual void prepareEnvironment();
    virtual ~SheetCompilerProgram() = default;
protected:
    virtual void prepareContext();
    virtual void printIntro(std::ostream &os);
    virtual void prepareSearchPaths();
    virtual void addSearchPath(const com::String &path);
    virtual void printSearchPaths() const;
    virtual void compile();
private:
    com::Path prepareJSONInput(const std::string &base64JsonInputStr);
};

#endif
