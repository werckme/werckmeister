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
#include <boost/di/extension/scopes/scoped.hpp>
#include <compiler/SheetNavigator.h>
#include <conductor/ConductionsPerformer.h>
#include "FactoryConfig.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


// #define LOCAL_TEST_RUN

typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;

class JsProgramOptions : public ICompilerProgramOptions
{
public:
	virtual bool isHelpSet() const { return false; }
	virtual bool isInputSet() const { return true; }
	virtual fm::String getInput() const { return input; }
	virtual bool isOutputSet() const { return false; }
	virtual bool isNoMetaSet() const { return false; }
	virtual fm::String getOutput() const { return ""; }
	virtual bool isJsonModeSet() const { return true; }
	virtual bool isJsonDocInfoMode() const { return false; }
	virtual bool isVersionSet() const { return false; }
	virtual void printHelpText(std::ostream &os) {}
	virtual bool isVerboseSet() const { return false; }
	virtual bool isDebugSet() const { return false; }
	virtual bool isBeginSet() const { return begin > 0; }
	virtual double getBegin() const { return begin; }
	virtual bool isEndSet() const { return false; }
	virtual double getEnd() const { return 0; }	
	std::string input;
	double begin = -1;
};


const char * create_c_str(const std::string &input) 
{
	auto bff = new char[input.length() + 1];
	strcpy(&bff[0], input.c_str());
	bff[input.length() - 1] = '\0';
	return &bff[0];
}


/**
 * usage:
 * let createCompileResult = cwrap('create_compile_result', 'number', ['string', 'number']);
 * let pCompilerResult = createCompileResult(jsonString, 0)
 * let jsonResult = UTF8ToString(pCompilerResult)
 * _free(pCompilerResult)
 */
extern "C" const char * create_compile_result(const char *file, double beginQuarters)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	namespace co = sheet::conductor;
	auto programOptionsPtr = std::make_shared<JsProgramOptions>();
	programOptionsPtr->input = file;
	programOptionsPtr->begin = beginQuarters;

	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	auto logger = std::make_shared<WarningsCollectorWithConsoleLogger>();
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()			.to<cp::DocumentParser>()			.in(di::extension::scoped)
		, di::bind<cp::ICompiler>()					.to<cp::Compiler>()					.in(di::extension::scoped)
		, di::bind<cp::ISheetTemplateRenderer>()	.to<cp::SheetTemplateRenderer>()	.in(di::extension::scoped)
		, di::bind<cp::ASheetEventRenderer>()		.to<cp::SheetEventRenderer>()		.in(di::extension::scoped)
		, di::bind<cp::IContext>()					.to<cp::MidiContext>()				.in(di::extension::scoped)
		, di::bind<cp::IPreprocessor>()				.to<cp::Preprocessor>()				.in(di::extension::scoped)
		, di::bind<cp::ISheetNavigator>()			.to<cp::SheetNavigator>()			.in(di::extension::scoped)
		, di::bind<co::IConductionsPerformer>()		.to<co::ConductionsPerformer>()		.in(di::extension::scoped)
		, di::bind<ICompilerProgramOptions>()		.to(programOptionsPtr)
		, di::bind<sheet::Document>()				.to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()		.to<fm::DefinitionsServer>()		.in(di::extension::scoped)
		, di::bind<fm::midi::Midi>()				.to(midiFile)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr 
		{
			return injector.template create<std::unique_ptr<fmapp::JsonWriter>>();
		})
		, di::bind<cp::ICompilerVisitor>()                      .to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			return injector.template create< std::shared_ptr<fmapp::DefaultTimeline>>();
		})
		, di::bind<fm::ILogger>()					.to(logger)
	);
	sheet::FactoryConfig factory(injector);
	factory.init();
	auto program = injector.create<SheetCompilerProgramJs>();
	auto jsonWriterPtr = std::dynamic_pointer_cast<fmapp::JsonWriter>(program.documentWriter());
	if (!jsonWriterPtr) {
		return nullptr;
	}
	std::stringstream outputStream;
	jsonWriterPtr->setOutputStream(outputStream);
	program.prepareEnvironment();
	program.execute();
	auto result = create_c_str(outputStream.str());
	documentPtr.reset();
	midiFile.reset();
	logger.reset();
	return result;
}


#ifdef LOCAL_TEST_RUN
int main(int argc, const char** argv)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	auto result = create_compile_result("put a valid sheetfile here", 0);
    std::cout << std::string(result) << std::endl;
	delete result;
}
#else
int main(int argc, const char** argv)
{
	std::cout << fm::getWerckmeister().version() << std::endl;
}
#endif