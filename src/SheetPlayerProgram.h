#ifndef SHEET_PLAYER_PROGRAM_HPP
#define SHEET_PLAYER_PROGRAM_HPP

#include <SheetCompilerProgram.h>

class SheetPlayerProgram : public SheetCompilerProgram {
private:
    ICompilerProgramOptionsPtr _programOptions;
    fm::ILoggerPtr _logger;
    sheet::compiler::IDocumentParserPtr  _documentParser;
    sheet::compiler::ICompilerPtr        _compiler;
    sheet::compiler::MidiContextPtr      _context;
    sheet::compiler::IPreprocessorPtr    _preprocessor;
    fm::midi::MidiPtr                    _midiFile;
    fmapp::IDocumentWriterPtr            _documentWriter;

public:
    typedef SheetCompilerProgram Base;
    SheetPlayerProgram(
        ICompilerProgramOptionsPtr               programOptions,
        fm::ILoggerPtr                           logger,
        sheet::compiler::IDocumentParserPtr      documentParser,
        sheet::compiler::ICompilerPtr            compiler,
        sheet::compiler::MidiContextPtr          context,
        sheet::compiler::IPreprocessorPtr        preprocessor,
        fm::midi::MidiPtr                        midiFile,
        fmapp::IDocumentWriterPtr                documentWriter
    ) : Base(programOptions, logger, documentParser, compiler, context, preprocessor, midiFile, documentWriter)
    {
    }
    virtual ~SheetPlayerProgram() = default;
protected:
private:
};

#endif
