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
    fm::midi::MidiPtr                            _midiFile;
    sheet::compiler::IContextPtr                 _context;
public:
    typedef SheetCompilerProgram Base;
    SheetPlayerProgram(
        IPlayerProgramOptionsPtr                 programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::MidiContextPtr          context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::MidiPlayerPtr                     midiPlayer
    ) : 
        Base(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, midiPlayer)
      , _programOptions(programOptions)
      , _logger(logger)
      , _midiPlayerPtr(midiPlayer)
      , _midiFile(midiFile)
      , _context(context)
    {
    }
    virtual ~SheetPlayerProgram() = default;
    virtual void onSheetChanged();
protected:
private:
};

#endif
