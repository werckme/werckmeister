#ifndef SHEET_COMPILER_PROGRAMJS_HPP
#define SHEET_COMPILER_PROGRAMJS_HPP

#include "SheetCompilerProgram.h"

class SheetCompilerProgramJs : public SheetCompilerProgram {
public:
    SheetCompilerProgramJs(
        ICompilerProgramOptionsPtr               programOptions,
        com::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::IContextPtr             context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        com::midi::MidiPtr                        midiFile,
        app::IDocumentWriterPtr                documentWriter,
        sheet::conductor::IConductionsPerformerPtr          conductionsPerformer
    ) : SheetCompilerProgram(programOptions
        , logger
        , documentParser
        , compiler
        , context
        , preprocessor
        , midiFile
        , documentWriter
        , conductionsPerformer
    )
    {
    }
    app::IDocumentWriterPtr documentWriter() const { return _documentWriter; }
    virtual ~SheetCompilerProgramJs() = default;

protected:
    virtual void prepareSearchPaths();
};

#endif
