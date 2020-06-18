#ifndef SHEET_PLAYER_PROGRAM_HPP
#define SHEET_PLAYER_PROGRAM_HPP

#include <SheetCompilerProgram.h>
#include <IPlayerProgramOptions.h>
#include <fmapp/ISheetWatcherHandler.h>

class SheetPlayerProgram : public SheetCompilerProgram, public fmapp::ISheetWatcherHandler {
private:
    IPlayerProgramOptionsPtr                     _programOptions;
    fm::ILoggerPtr                               _logger;
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
        fmapp::IDocumentWriterPtr                documentWriter
    ) : 
        Base(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, documentWriter)
      , _programOptions(programOptions)
      , _logger(logger)
    {
    }
    virtual ~SheetPlayerProgram() = default;
    virtual void onSheetChanged();
protected:
private:
};

#endif
