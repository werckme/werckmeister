#pragma once

#include "SheetCompilerProgram.h"

class SheetCompilerProgramJs : public SheetCompilerProgram
{
public:
    SheetCompilerProgramJs(
        ICompilerProgramOptionsPtr programOptions,
        com::ILoggerPtr logger,
        parser::IDocumentParserPtr documentParser,
        compiler::ICompilerPtr compiler,
        compiler::IContextPtr context,
        compiler::IPreprocessorPtr preprocessor,
        com::midi::MidiPtr midiFile,
        app::IDocumentWriterPtr documentWriter,
        conductor::IConductionsPerformerPtr conductionsPerformer) : SheetCompilerProgram(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, documentWriter, conductionsPerformer)
    {
    }
    app::IDocumentWriterPtr documentWriter() const { return _documentWriter; }
    virtual ~SheetCompilerProgramJs() = default;

protected:
    virtual void prepareSearchPaths();
};
