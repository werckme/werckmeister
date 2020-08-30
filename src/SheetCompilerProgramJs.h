#ifndef SHEET_COMPILER_PROGRAMJS_HPP
#define SHEET_COMPILER_PROGRAMJS_HPP

#include "SheetCompilerProgram.h"

class SheetCompilerProgramJs : public SheetCompilerProgram {
private:
    ICompilerProgramOptionsPtr _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::IContextPtr         _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
    fm::midi::MidiPtr                    _midiFile;
    fmapp::IDocumentWriterPtr            _documentWriter;

public:
    SheetCompilerProgramJs(
        ICompilerProgramOptionsPtr               programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::IContextPtr             context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::IDocumentWriterPtr                documentWriter
    ) : SheetCompilerProgram(programOptions
        , logger
        , documentParser
        , compiler
        , context
        , preprocessor
        , midiFile
        , documentWriter
    )
    {
    }
    virtual ~SheetCompilerProgramJs() = default;
protected:
    virtual void prepareSearchPaths();
};

#endif
