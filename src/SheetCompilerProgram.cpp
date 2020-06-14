#include "SheetCompilerProgram.h"
#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "compiler/Compiler.h"
#include "parser/parser.h"
#include "compiler/context/MidiContext.h"
#include "sheet.h"
#include "compiler/SheetEventRenderer.h"
#include <fmapp/os.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <exception>
#include <fm/config.hpp>
#include "DiConfig.h"

void SheetCompilerProgram::prepareEnvironment()
{
    if (_programOptions->isVerboseSet()) {
        _logger->logLevel(fm::ILogger::LevelBabble);
    }
    _logger->babble(WRMLogLambda(printIntro(log)));
    prepareSearchPaths();
    prepareContext();
}

void SheetCompilerProgram::printIntro(std::ostream &os)
{
    os << "\tWERCKMEISTER "                                   << std::endl
       << "\t(c) Samba Godschynski "                          << std::endl
       << "\thttps://werckme.github.io"                       << std::endl
       << "\tversion: "                   << SHEET_VERSION    << std::endl
       << "\tMIDI ppq value: "            << fm::PPQ          << std::endl
    ;
}

void SheetCompilerProgram::prepareContext()
{
    auto midiFile = fm::getWerckmeister().createMidi();
    _context->midi(midiFile);
}

void SheetCompilerProgram::compile()
{
    if (_programOptions->isHelpSet()) {
        _programOptions->printHelpText(std::cout);
        std::cout << std::endl;
        return;
    }
    if (!_programOptions->isInputSet()) {
        throw std::runtime_error("missing input file");
    }
    auto file = _programOptions->getInput();
    _logger->babble(WRMLogLambda(log << "parsing '" << file << "'"));
    auto document =_documentParser->parse(file);
    _logger->babble(WRMLogLambda(log << "preprocess '" << file << "'"));
    _preprocessor->preprocess(document);
    _logger->babble(WRMLogLambda(log << "compiling '" << file << "'"));    
    _compiler->compile(document);
}

void SheetCompilerProgram::prepareSearchPaths()
{
    using boost::filesystem::path;
    using boost::filesystem::system_complete;
    auto &wm = fm::getWerckmeister();
    auto execPath = path(fmapp::os::getExecutablePath());
    wm.addSearchPath(execPath.string());
    wm.addSearchPath(system_complete(execPath / path("../share/werckmeister")).string());
#ifndef WIN32
    wm.addSearchPath((path("/usr/local/share/werckmeister").string()));
#endif
}

int SheetCompilerProgram::execute() {
    try {
        compile();
        return 0;
    }
    catch (const fm::Exception &ex)
	{
        std::cerr << ex.toString() << std::endl;
	}
	catch (const std::exception &ex)
	{
        std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
        std::cerr << "unkown error" << std::endl;
	}
    return -1;
}