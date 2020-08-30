#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetCompilerProgramJs.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <fm/ConsoleLogger.h>
#include <compiler/LoggerAndWarningsCollector.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <compiler/EventLogger.h>
#include <sheet/Document.h>
#include <fm/DefinitionsServer.h>
#include <fm/midi.hpp>
#include <fmapp/MidiFileWriter.h>
#include <fmapp/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <fmapp/TimelineVisitor.hpp>

typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;
int compile(const std::string &json);

class JsProgramOptions : public ICompilerProgramOptions
{
public:
	virtual bool isHelpSet() const { return false; }
	virtual bool isInputSet() const { return true; }
	virtual fm::String getInput() const { return inputJson; }
	virtual bool isOutputSet() const { return false; }
	virtual bool isNoMetaSet() const { return true; }
	virtual fm::String getOutput() const { return ""; }
	virtual bool isJsonModeSet() const { return true; }
	virtual bool isJsonDocInfoMode() const { return false; }
	virtual bool isVersionSet() const { return false; }
	virtual void printHelpText(std::ostream &os) {}
	virtual bool isVerboseSet() const { return false; }
	virtual bool isDebugSet() const { return false; }
	std::string inputJson;
};


int main(int argc, const char** argv)
{
    std::cout << "HELLO" << std::endl;
	compile("[{\"path\":\"unknown.sheet\",\"data\":\"tempo: 140;device: MyDevice  midi 1;instrumentDef:ex1  MyDevice  0 0 0;[instrument: ex1;{c'4   g8 g   a4 g~  | \n   g  b c'}]\"}]");
}

int compile(const std::string &json)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	auto programOptionsPtr = std::make_shared<JsProgramOptions>();
	programOptionsPtr->inputJson = json;

	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isJsonModeSet();
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()			.to<cp::DocumentParser>()			.in(di::singleton)
		, di::bind<cp::ICompiler>()					.to<cp::Compiler>()					.in(di::singleton)
		, di::bind<cp::ISheetTemplateRenderer>()	.to<cp::SheetTemplateRenderer>()	.in(di::singleton)
		, di::bind<cp::ASheetEventRenderer>()		.to<cp::SheetEventRenderer>()		.in(di::singleton)
		, di::bind<cp::IContext>()					.to<cp::MidiContext>()				.in(di::singleton)
		, di::bind<cp::IPreprocessor>()				.to<cp::Preprocessor>()				.in(di::singleton)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()		.to<fm::DefinitionsServer>()		.in(di::singleton)
		, di::bind<fm::midi::Midi>()				.to(midiFile)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr 
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::shared_ptr<fmapp::JsonWriter>>();
			}
			return injector.template create<std::shared_ptr<fmapp::MidiFileWriter>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<fmapp::DefaultTimeline>>();
			}
			return injector.template create< std::shared_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<fm::ILogger>()					.to([&](const auto &injector) -> fm::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
	);
	auto program = injector.create<SheetCompilerProgramJs>();
	program.prepareEnvironment();
	return program.execute();
}