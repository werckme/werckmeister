#ifndef SHEET_COMPILER_PROGRAMJS_HPP
#define SHEET_COMPILER_PROGRAMJS_HPP

#include "SheetCompilerProgram.h"

class SheetCompilerProgramJs : public SheetCompilerProgram {
public:
    SheetCompilerProgramJs(
        ICompilerProgramOptionsPtr               programOptions,
        com::ILoggerPtr                           logger,
        documentModel::compiler::IDocumentParserPtr      documentParser,
        documentModel::compiler::ICompilerPtr            compiler,
        documentModel::compiler::IContextPtr             context,
        documentModel::compiler::IPreprocessorPtr        preprocessor,
        com::midi::MidiPtr                        midiFile,
        app::IDocumentWriterPtr                documentWriter,
        documentModel::conductor::IConductionsPerformerPtr          conductionsPerformer
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
