#ifndef SHEET_PLAYER_PROGRAM_HPP
#define SHEET_PLAYER_PROGRAM_HPP

#include <SheetCompilerProgram.h>
#include <IPlayerProgramOptions.h>
#include <fmapp/ISheetWatcherHandler.h>
#include <fmapp/MidiPlayer.h>

class SheetPlayerProgram : public SheetCompilerProgram, public fmapp::ISheetWatcherHandler {
private:
    IPlayerProgramOptionsPtr                     _programOptions;
    fm::ILoggerPtr                               _logger;
    fmapp::MidiPlayerPtr                         _midiPlayerPtr;
public:
    enum { 
        RetCodeRestart = 100 // restart right after exiting, for example when a document was changed
    };
    typedef SheetCompilerProgram Base;
    SheetPlayerProgram(
        ICompilerProgramOptionsPtr               programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::IContextPtr             context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::IDocumentWriterPtr                documentWriter
    ) : 
        Base(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, documentWriter)
      , _programOptions(std::dynamic_pointer_cast<IPlayerProgramOptions>(programOptions))
      , _logger(logger)
      , _midiPlayerPtr(std::dynamic_pointer_cast<fmapp::MidiPlayer>(documentWriter))
    {
    }
    virtual ~SheetPlayerProgram() = default;
    virtual void onSheetChanged();
    virtual int execute() override;
protected:
private:
    bool documentWasChanged = false;
};

#endif
