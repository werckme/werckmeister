#ifndef SHEET_COMPILER_PROGRAM_HPP
#define SHEET_COMPILER_PROGRAM_HPP

#include <parser/IDocumentParser.h>
#include <fm/common.hpp>
#include <forward.hpp>
#include "ICompilerProgramOptions.h"
#include <compiler/ICompiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/IPreprocessor.h>
#include <conductor/IConductionsPerformer.h>
#include <fm/ILogger.h>
#include <ostream>
#include <fmapp/IDocumentWriter.h>

class SheetCompilerProgram {
protected:
    ICompilerProgramOptionsPtr _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::IContextPtr         _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
    fm::midi::MidiPtr                    _midiFile;
    fmapp::IDocumentWriterPtr            _documentWriter;
    sheet::conductor::IConductionsPerformerPtr      _conductionsPerformer;

public:
    SheetCompilerProgram(
        ICompilerProgramOptionsPtr               programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::IContextPtr             context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::IDocumentWriterPtr                documentWriter,
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
    virtual void addSearchPath(const fm::String &path);
    virtual void printSearchPaths() const;
    virtual void compile();
private:
    fm::Path prepareJSONInput(const std::string &base64JsonInputStr);
};

#endif
