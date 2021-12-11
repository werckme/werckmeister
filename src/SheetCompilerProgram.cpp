#include "SheetCompilerProgram.h"
#include <com/werckmeister.hpp>
#include <com/common.hpp>
#include "sheet/Document.h"
#include "compiler/Compiler.h"
#include "parser/parser.h"
#include "compiler/context/MidiContext.h"
#include "sheet.h"
#include "compiler/SheetEventRenderer.h"
#include <app/os.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <exception>
#include <com/config.hpp>
#include <app/JsonStringInputReader.hpp>
#include <boost/algorithm/string/join.hpp>
#include <compiler/error.hpp>

void SheetCompilerProgram::prepareEnvironment()
{
    if (_programOptions->isDebugSet()) {
        _logger->logLevel(com::ILogger::LevelDebug);
    }
    else if (_programOptions->isVerboseSet()) {
        _logger->logLevel(com::ILogger::LevelBabble);
    }
    _logger->babble(WMLogLambda(printIntro(log)));
    prepareSearchPaths();
    prepareContext();
}

void SheetCompilerProgram::printIntro(std::ostream &os)
{
    static bool shown = false;
    if (shown) {
        return;
    }
    shown = true;
    os << "\tWERCKMEISTER "                                   << std::endl
       << "\t(c) Samba Godschynski "                          << std::endl
       << "\thttps://werckme.github.io"                       << std::endl
       << "\tversion: "                   << SHEET_VERSION    << std::endl
       << "\tMIDI ppq value: "            << com::PPQ          << std::endl
    ;
}

void SheetCompilerProgram::prepareContext()
{
}

void SheetCompilerProgram::compile()
{
    _midiFile->midiConfig.skipMetaEvents = _programOptions->isNoMetaSet();
    auto file = _programOptions->getInput();
    _logger->babble(WMLogLambda(log << "parsing '" << file << "'"));
    auto document =_documentParser->parse(file);
    if (_logger->logLevel() >= com::ILogger::LevelBabble) {
        printSearchPaths();
    }
    _logger->babble(WMLogLambda(log << "compiling '" << file << "'"));    
    _compiler->compile(document);
    try {
        _logger->babble(WMLogLambda(log << "aplying conduction rules"));
        _conductionsPerformer->applyConductions();
        if (_programOptions->isBeginSet() || _programOptions->isEndSet()) {
            auto beginTicks = _programOptions->isBeginSet() ? _programOptions->getBegin() * com::PPQ : 0;
            auto endTicks = _programOptions->isEndSet() ? _programOptions->getEnd() * com::PPQ : com::Ticks(INT_MAX);
            _midiFile->crop(beginTicks, endTicks);
        }
        _midiFile->seal();
    } catch(com::Exception &ex) {
        ex << sheet::compiler::ex_sheet_document(document);
        throw;
    }
    _logger->babble(WMLogLambda(log << "write document"));   
    _documentWriter->write(document);
}

void SheetCompilerProgram::addSearchPath(const com::String &path)
{
    auto &wm = com::getWerckmeister();
    wm.addSearchPath(path);
}

void SheetCompilerProgram::prepareSearchPaths()
{
    using boost::filesystem::path;
    using boost::filesystem::system_complete;
    
    auto execPath = path(app::os::getExecutablePath());
    addSearchPath(execPath.string());
    addSearchPath(system_complete(execPath / path("../share/werckmeister")).string());
#ifndef WIN32
    addSearchPath((path("/usr/local/share/werckmeister").string()));
#endif
}

void SheetCompilerProgram::printSearchPaths() const
{
    const auto& paths = com::getWerckmeister().searchPaths();
    auto strSearchPaths = boost::algorithm::join(paths, "\n");
    _logger->babble(WMLogLambda(log << "search paths:" << std::endl << strSearchPaths << std::endl));
}

int SheetCompilerProgram::execute() { 
    try { 
        if (_programOptions->isHelpSet()) {
            _programOptions->printHelpText(std::cout);
            std::cout << std::endl;
            return 0;
        }  
        if (_programOptions->isVersionSet()) {
            auto &wm = com::getWerckmeister();
            std::cout << wm.version() << std::endl;
            return 0;
        }
        if (!_programOptions->isInputSet()) {
            throw std::runtime_error("missing input file");
        }                              
        compile();
        return 0;
    }
    catch (const com::Exception &ex)
	{
        _documentWriter->writeException(ex);
	}
	catch (const std::exception &ex)
	{
        _documentWriter->writeException(ex);
	}
	catch (...)
	{
        _documentWriter->writeUnknownException();
	}
    return -1;
}