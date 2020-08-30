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
#include <fmapp/JsonStringInputReader.hpp>
#include <boost/algorithm/string/join.hpp>

void SheetCompilerProgram::prepareEnvironment()
{
    if (_programOptions->isDebugSet()) {
        _logger->logLevel(fm::ILogger::LevelDebug);
    }
    else if (_programOptions->isVerboseSet()) {
        _logger->logLevel(fm::ILogger::LevelBabble);
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
    if (_programOptions->isJsonModeSet()) { // for more cases, cosider a input provider approach
        auto base64JsonInputStr = file;
        file = prepareJSONInput(base64JsonInputStr);
    }
    _logger->babble(WMLogLambda(log << "parsing '" << file << "'"));
    auto document =_documentParser->parse(file);
    if (_logger->logLevel() >= fm::ILogger::LevelBabble) {
        printSearchPaths();
    }
    _logger->babble(WMLogLambda(log << "compiling '" << file << "'"));    
    _compiler->compile(document);
    _logger->babble(WMLogLambda(log << "write document"));   
    _documentWriter->write(document);
}

void SheetCompilerProgram::addSearchPath(const fm::String &path)
{
    auto &wm = fm::getWerckmeister();
    wm.addSearchPath(path);
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

void SheetCompilerProgram::printSearchPaths() const
{
    const auto& paths = fm::getWerckmeister().searchPaths();
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

fm::Path SheetCompilerProgram::prepareJSONInput(const std::string &inputJson) 
{
	auto &wm = fm::getWerckmeister();
	// // prepare vfs
	fmapp::JsonStringInputReader jsonReader;
	auto vfiles = jsonReader.readVirtualFS(inputJson);
	fm::Path sheetPath;
	for(const auto &vfile : vfiles) {
		if (wm.fileIsSheet(vfile.path)) {
			sheetPath = vfile.path;
		}
		wm.createVirtualFile(vfile.path, vfile.data);
	}
	return sheetPath;
}