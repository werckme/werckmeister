#pragma once

#include <SheetCompilerProgram.h>
#include <IPlayerProgramOptions.h>
#include <app/ISheetWatcherHandler.h>
#include <app/MidiPlayer.h>

class SheetPlayerProgram : public SheetCompilerProgram, public app::ISheetWatcherHandler
{
private:
    IPlayerProgramOptionsPtr _programOptions;
    com::ILoggerPtr _logger;
    app::MidiPlayerPtr _midiPlayerPtr;

public:
    enum
    {
        RetCodeRestart = 100 // restart right after exiting, for example when a document was changed
    };
    typedef SheetCompilerProgram Base;
    SheetPlayerProgram(
        ICompilerProgramOptionsPtr programOptions,
        com::ILoggerPtr logger,
        parser::IDocumentParserPtr documentParser,
        compiler::ICompilerPtr compiler,
        compiler::IContextPtr context,
        compiler::IPreprocessorPtr preprocessor,
        com::midi::MidiPtr midiFile,
        app::IDocumentWriterPtr documentWriter,
        conductor::IConductionsPerformerPtr conductionsPerformer) : Base(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, documentWriter, conductionsPerformer), _programOptions(std::dynamic_pointer_cast<IPlayerProgramOptions>(programOptions)), _logger(logger), _midiPlayerPtr(std::dynamic_pointer_cast<app::MidiPlayer>(documentWriter))
    {
    }
    virtual ~SheetPlayerProgram() = default;
    virtual void onSheetChanged();
    virtual int execute() override;

protected:
private:
    bool documentWasChanged = false;
};
