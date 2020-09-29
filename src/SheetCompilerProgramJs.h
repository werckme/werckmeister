#ifndef SHEET_COMPILER_PROGRAMJS_HPP
#define SHEET_COMPILER_PROGRAMJS_HPP

#include "SheetCompilerProgram.h"

class SheetCompilerProgramJs : public SheetCompilerProgram {
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
    fmapp::IDocumentWriterPtr documentWriter() const { return _documentWriter; }
    virtual ~SheetCompilerProgramJs() = default;

protected:
    virtual void prepareSearchPaths();
};

#endif
