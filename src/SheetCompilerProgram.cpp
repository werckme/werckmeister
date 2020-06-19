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
#include "FactoryConfig.h"


void SheetCompilerProgram::prepareEnvironment()
{
    if (_programOptions->isVerboseSet()) {
        _logger->logLevel(fm::ILogger::LevelBabble);
    }
    _logger->babble(WMLogLambda(printIntro(log)));
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
}

void SheetCompilerProgram::compile()
{
    _midiFile->midiConfig.skipMetaEvents = _programOptions->isNoMetaSet();
    auto file = _programOptions->getInput();
    _logger->babble(WMLogLambda(log << "parsing '" << file << "'"));
    auto document =_documentParser->parse(file);
    _logger->babble(WMLogLambda(log << "preprocess '" << file << "'"));
    _preprocessor->preprocess(document);
    _logger->babble(WMLogLambda(log << "compiling '" << file << "'"));    
    _compiler->compile(document);
    _logger->babble(WMLogLambda(log << "write document"));   
    _documentWriter->write(document);
}

void SheetCompilerProgram::addSearchPath(const fm::String &path)
{
    auto &wm = fm::getWerckmeister();
    wm.addSearchPath(path);
     _logger->babble(WMLogLambda(log << "add to search path: \"" << path << "\""));   
}

void SheetCompilerProgram::prepareSearchPaths()
{
    using boost::filesystem::path;
    using boost::filesystem::system_complete;
    
    auto execPath = path(fmapp::os::getExecutablePath());
    addSearchPath(execPath.string());
    addSearchPath(system_complete(execPath / path("../share/werckmeister")).string());
#ifndef WIN32
    addSearchPath((path("/usr/local/share/werckmeister").string()));
#endif
}

int SheetCompilerProgram::execute() { 
    try { 
        if (_programOptions->isHelpSet()) {
            _programOptions->printHelpText(std::cout);
            std::cout << std::endl;
            return 0;
        }  
        if (_programOptions->isVersionSet()) {
            auto &wm = fm::getWerckmeister();
            std::cout << wm.version() << std::endl;
            return 0;
        }
        if (!_programOptions->isInputSet()) {
            throw std::runtime_error("missing input file");
        }                              
        compile();
        return 0;
    }
    catch (const fm::Exception &ex)
	{
        _logger->error(WMLogLambda(log << ex.toString() ));
	}
	catch (const std::exception &ex)
	{
        _logger->error(WMLogLambda(log << ex.what() ));
	}
	catch (...)
	{
        _logger->error(WMLogLambda(log << "unkown error" ));
	}
    return -1;
}