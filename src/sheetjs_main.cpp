#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include "SheetCompilerProgramJs.h"
#include <parser/parser.h>
#include <com/werckmeister.hpp>
#include <CompilerProgramOptions.h>
#include <com/ConsoleLogger.h>
#include <compiler/LoggerAndWarningsCollector.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/SheetTemplateRenderer.h>
#include <compiler/Compiler.h>
#include <compiler/context/MidiContext.h>
#include <compiler/Preprocessor.h>
#include <compiler/EventLogger.h>
#include <documentModel/Document.h>
#include <compiler/DefinitionsServer.h>
#include <com/midi.hpp>
#include <app/MidiFileWriter.h>
#include <app/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <app/TimelineVisitor.hpp>
#include <boost/di/extension/scopes/scoped.hpp>
#include <compiler/SheetNavigator.h>
#include <conductor/ConductionsPerformer.h>
#include "FactoryConfig.h"
#include <compiler/CompoundVisitor.hpp>
#include <compiler/EventInformationServer.h>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// #define LOCAL_TEST_RUN

typedef compiler::EventLogger<com::ConsoleLogger> LoggerImpl;
typedef compiler::LoggerAndWarningsCollector<com::ConsoleLogger> WarningsCollectorWithConsoleLogger;

class JsProgramOptions : public ICompilerProgramOptions
{
public:
	virtual bool isHelpSet() const override { return false; }
	virtual bool isInputSet() const override  { return true; }
	virtual com::String getInput() const override  { return input; }
	virtual bool isOutputSet() const override  { return false; }
	virtual bool isNoMetaSet() const override  { return false; }
	virtual com::String getOutput() const override  { return ""; }
	virtual bool isJsonModeSet() const override  { return true; }
	virtual bool isJsonDocInfoMode() const override  { return false; }
	virtual bool isJsonDebugInfoMode() const override  { return false; }
	virtual bool isVersionSet() const override  { return false; }
	virtual void printHelpText(std::ostream &os) override {}
	virtual bool isVerboseSet() const override  { return false; }
	virtual bool isDebugSet() const override  { return false; }
	virtual bool isBeginSet() const override  { return begin > 0; }
	virtual double getBegin() const override  { return begin; }
	virtual bool isEndSet() const override  { return false; }
	virtual double getEnd() const override  { return 0; }
	virtual com::String logFile() const override { return ""; }
	std::string input;
	double begin = -1;
};

const char *create_c_str(const std::string &input)
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
extern "C" const char *create_compile_result(const char *file, double beginQuarters)
{
	namespace di = boost::di;
	namespace cp = compiler;
	namespace co = conductor;
	namespace pr = parser;
	auto programOptionsPtr = std::make_shared<JsProgramOptions>();
	programOptionsPtr->input = file;
	programOptionsPtr->begin = beginQuarters;

	auto documentPtr = std::make_shared<documentModel::Document>();
	auto midiFile = com::getWerckmeister().createMidi();
	auto logger = std::make_shared<WarningsCollectorWithConsoleLogger>();
	auto injector = di::make_injector(
		di::bind<pr::IDocumentParser>().to<pr::DocumentParser>().in(di::extension::scoped), 
		di::bind<cp::ICompiler>().to<cp::Compiler>().in(di::extension::scoped), 
		di::bind<cp::ISheetTemplateRenderer>().to<cp::SheetTemplateRenderer>().in(di::extension::scoped), 
		di::bind<cp::ASheetEventRenderer>().to<cp::SheetEventRenderer>().in(di::extension::scoped),
		di::bind<cp::IContext>().to<cp::MidiContext>().in(di::extension::scoped), 
		di::bind<cp::IPreprocessor>().to<cp::Preprocessor>().in(di::extension::scoped), 
		di::bind<cp::ISheetNavigator>().to<cp::SheetNavigator>().in(di::extension::scoped), 
		di::bind<co::IConductionsPerformer>().to<co::ConductionsPerformer>().in(di::extension::scoped),
		di::bind<cp::IEventInformationServer>().to<cp::EventInformationServer>().in(di::singleton),
		di::bind<ICompilerProgramOptions>().to(programOptionsPtr), di::bind<documentModel::Document>().to(documentPtr), 
		di::bind<compiler::IDefinitionsServer>().to<compiler::DefinitionsServer>().in(di::extension::scoped), 
		di::bind<com::midi::Midi>().to(midiFile), di::bind<app::IDocumentWriter>().to([&](const auto &injector) -> app::IDocumentWriterPtr
		{ 
			return injector.template create<std::unique_ptr<app::JsonWriter>>(); 
		}),
		di::bind<cp::ICompilerVisitor>().to([&](const auto &injector) -> cp::ICompilerVisitorPtr
		{ 
			return injector.template create<std::shared_ptr< cp::CompoundVisitor_N2<app::DefaultTimeline, cp::EventInformationServer> >>();
		}),
		di::bind<com::ILogger>().to(logger));
	FactoryConfig factory(injector);
	factory.init();
	auto program = injector.create<SheetCompilerProgramJs>();
	auto jsonWriterPtr = std::dynamic_pointer_cast<app::JsonWriter>(program.documentWriter());
	if (!jsonWriterPtr)
	{
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
int main(int argc, const char **argv)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	auto result = create_compile_result("put a valid sheetfile here", 0);
	std::cout << std::string(result) << std::endl;
	delete result;
}
#else
int main(int argc, const char **argv)
{
	std::cout << com::getWerckmeister().version() << std::endl;
}
#endif