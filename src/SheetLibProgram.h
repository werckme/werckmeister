#pragma once

#include "SheetCompilerProgram.h"

class SheetLibProgram : public SheetCompilerProgram
{
protected:
public:
    typedef SheetCompilerProgram Base;
    SheetLibProgram(
        ICompilerProgramOptionsPtr programOptions,
        com::ILoggerPtr logger,
        parser::IDocumentParserPtr documentParser,
        compiler::ICompilerPtr compiler,
        compiler::IContextPtr context,
        compiler::IPreprocessorPtr preprocessor,
        com::midi::MidiPtr midiFile,
        app::IDocumentWriterPtr documentWriter,
        conductor::IConductionsPerformerPtr conductionsPerformer) : Base
        (
            programOptions,
            logger,
            documentParser,
            compiler,
            context,
            preprocessor,
            midiFile,
            documentWriter,
            conductionsPerformer
        )
    {
    }
    virtual ~SheetLibProgram() = default;
    com::midi::MidiPtr getMidiFile() const { return _midiFile; }
    virtual int execute() override;
};
