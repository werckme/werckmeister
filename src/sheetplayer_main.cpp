
#include <boost/di.hpp>
#include <boost/di/extension/scopes/scoped.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "SheetPlayerProgram.h"
#include <parser/parser.h>
#include <fm/werckmeister.hpp>
#include <PlayerProgramOptions.h>
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
#include <fmapp/MidiPlayer.h>
#include <fmapp/JsonWriter.h>
#include <compiler/DefaultCompilerVisitor.h>
#include <fmapp/TimelineVisitor.hpp>
#include <fmapp/SheetWatcher.h>
#include <fmapp/PlayerTimePrinter.h>
#include <fmapp/DiContainerWrapper.h>
#include <fmapp/ISheetWatcherHandler.h>
#include <fmapp/Funkfeuer.h>
#include <fmapp/UdpSender.hpp>
#include <fmapp/NullStringSender.hpp>
#include <boost/dll.hpp>
#include <fluidsynth.h>

#ifdef SHEET_USE_BOOST_TIMER
#include "fmapp/boostTimer.h"
#else
#include "fmapp/os.hpp"
#endif


typedef sheet::compiler::EventLogger<fm::ConsoleLogger> 			   LoggerImpl;
typedef sheet::compiler::LoggerAndWarningsCollector<fm::ConsoleLogger> WarningsCollectorWithConsoleLogger;

int startPlayer(std::shared_ptr<PlayerProgramOptions> programOptionsPtr);

funk::UdpSenderPtr _udpSender = nullptr;



#include <fluidsynth.h>
#if defined(WIN32)
#include <windows.h>
#define sleep(_t) Sleep(_t * 1000)
#else
#include <stdlib.h>
#include <unistd.h>
#endif
int test_fluidsynth()
{
	boost::dll::shared_library lib("libfluidsynth-2.dll");
	auto dll_new_fluid_settings			= lib.get<fluid_settings_t* ()>("new_fluid_settings");
	auto dll_new_fluid_synth			= lib.get<fluid_synth_t* (fluid_settings_t*)>("new_fluid_synth");
	auto dll_new_fluid_audio_driver		= lib.get<fluid_audio_driver_t* (fluid_settings_t*, fluid_synth_t*)>("new_fluid_audio_driver");
	auto dll_fluid_synth_sfload			= lib.get<int(fluid_synth_t*, const char*, int)>("fluid_synth_sfload");
	auto dll_fluid_synth_noteon			= lib.get<int(fluid_synth_t*, int, int, int)>("fluid_synth_noteon");
	auto dll_fluid_synth_noteoff		= lib.get<int(fluid_synth_t*, int, int)>("fluid_synth_noteoff");
	auto dll_fluid_synth_program_change = lib.get<int(fluid_synth_t*, int, int)>("fluid_synth_program_change");


	fluid_settings_t* settings;
	fluid_synth_t* synth;
	fluid_audio_driver_t* adriver;
	int sfont_id;
	int i, key;
	/* Create the settings. */
	settings = dll_new_fluid_settings();
	/* Change the settings if necessary*/
	/* Create the synthesizer. */
	synth = dll_new_fluid_synth(settings);
	/* Create the audio driver. The synthesizer starts playing as soon
	   as the driver is created. */
	adriver = dll_new_fluid_audio_driver(settings, synth);
	/* Load a SoundFont and reset presets (so that new instruments
	 * get used from the SoundFont) */
	sfont_id = dll_fluid_synth_sfload(synth, "E:\\Users\\samba\\Soundfonts\\FluidR3_GM.sf2", 1);
	if (sfont_id == FLUID_FAILED)
	{
		puts("Loading the SoundFont failed!");
		goto err;
	}
	/* Initialize the random number generator */
	srand(getpid());
	dll_fluid_synth_program_change(synth, 0, 16);
	for (i = 0; i < 12; i++)
	{
		/* Generate a random key */
		key = 60 + (int)(12.0f * rand() / (float)RAND_MAX);
		/* Play a note */
		dll_fluid_synth_noteon(synth, 0, key, 80);
		/* Sleep for 1 second */
		sleep(1);
		/* Stop the note */
		dll_fluid_synth_noteoff(synth, 0, key);
	}
err:
//	/* Clean up */
	auto dll_delete_fluid_audio_driver = lib.get<void(fluid_audio_driver_t*)>("delete_fluid_audio_driver");
	auto dll_delete_fluid_synth = lib.get<void(fluid_synth_t*)>("delete_fluid_synth");
	auto dll_delete_fluid_settings = lib.get<void(fluid_settings_t*)>("delete_fluid_settings");
	dll_delete_fluid_audio_driver(adriver);
	dll_delete_fluid_synth(synth);
	dll_delete_fluid_settings(settings);
	return 0;
}


int main(int argc, const char** argv)
{
	auto programOptionsPtr = std::make_shared<PlayerProgramOptions>();

	try {

		test_fluidsynth();

		programOptionsPtr->parseProgrammArgs(argc, argv);
		if (programOptionsPtr->isUdpSet()) {
			auto host = programOptionsPtr->getUdpHostname();
			_udpSender = std::make_shared<funk::UdpSender>(host);
			_udpSender->start();
		}
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	int returnCode = 0;
	
#ifdef SHEET_USE_BOOST_TIMER
       std::thread boost_asio_([] {
           fmapp::BoostTimer::io_run();
       });
#endif

	do {
		returnCode = startPlayer(programOptionsPtr);
	} while (returnCode == SheetPlayerProgram::RetCodeRestart);
	

#ifdef SHEET_USE_BOOST_TIMER
	fmapp::BoostTimer::io_stop();
	boost_asio_.join();
#endif
	if (_udpSender) {
		_udpSender->stop();
	}
	return returnCode;
}

int startPlayer(std::shared_ptr<PlayerProgramOptions> programOptionsPtr)
{
	namespace di = boost::di;
	namespace cp = sheet::compiler;
	fmapp::SheetWatcherHandlersPtr sheetWatcherHandlers = std::make_shared<fmapp::SheetWatcherHandlers>();
	auto documentPtr = std::make_shared<sheet::Document>();
	auto midiFile = fm::getWerckmeister().createMidi();
	bool needTimeline = programOptionsPtr->isUdpSet();
	fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr> loopVisitors;
	bool writeWarningsToConsole = !(programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode());
	auto injector = di::make_injector(
		  di::bind<cp::IDocumentParser>()									 .to<cp::DocumentParser>()			.in(di::extension::scoped)
		, di::bind<cp::ICompiler>()											 .to<cp::Compiler>()				.in(di::extension::scoped)
		, di::bind<cp::ISheetTemplateRenderer>()							 .to<cp::SheetTemplateRenderer>()	.in(di::extension::scoped)
		, di::bind<cp::ASheetEventRenderer>()								 .to<cp::SheetEventRenderer>()		.in(di::extension::scoped)
		, di::bind<cp::IContext>()											 .to<cp::MidiContext>()				.in(di::extension::scoped)
		, di::bind<cp::IPreprocessor>()										 .to<cp::Preprocessor>()			.in(di::extension::scoped)
		, di::bind<ICompilerProgramOptions>()								 .to(programOptionsPtr)
		, di::bind<sheet::Document>()										 .to(documentPtr)
		, di::bind<fm::IDefinitionsServer>()								 .to<fm::DefinitionsServer>()		.in(di::extension::scoped)
		, di::bind<fm::midi::Midi>()										 .to(midiFile)
		, di::bind<fmapp::SheetWatcherHandlers>()							 .to(sheetWatcherHandlers)
		, di::bind<fmapp::DiContainerWrapper<fmapp::IPlayerLoopVisitorPtr>>().to(loopVisitors)
		, di::bind<fmapp::IDocumentWriter>()		.to([&](const auto &injector) -> fmapp::IDocumentWriterPtr
		{
			if (programOptionsPtr->isJsonModeSet() || programOptionsPtr->isJsonDocInfoMode()) {
				return injector.template create<std::unique_ptr<fmapp::JsonWriter>>();
			}
			return injector.template create<std::unique_ptr<fmapp::MidiPlayer>>();
		})
		, di::bind<cp::ICompilerVisitor>()			.to([&](const auto &injector) -> cp::ICompilerVisitorPtr 
		{
			if (needTimeline) {
				return injector.template create< std::shared_ptr<fmapp::DefaultTimeline>>();
			}
			return injector.template create< std::unique_ptr<cp::DefaultCompilerVisitor>>();
		})
		, di::bind<fm::ILogger>()					.to([&](const auto &injector) -> fm::ILoggerPtr 
		{
			if (writeWarningsToConsole) {
				return injector.template create<std::shared_ptr<LoggerImpl>>();
			}
			return injector.template create<std::shared_ptr<WarningsCollectorWithConsoleLogger>>();
		})
		, di::bind<fmapp::IStringSender>()					.to([&](const auto &injector) -> fmapp::IStringSenderPtr 
		{
			if (programOptionsPtr->isUdpSet()) {
				return _udpSender;
			}
			return injector.template create<std::shared_ptr<fmapp::NullStringSender>>();
		})		
	);
	try {
		if (!programOptionsPtr->isNoTimePrintSet()) {
			// unique_ptr to shared_ptr is for purpose: create shared_ptr means singleton, but in this case we need unique
			std::shared_ptr<fmapp::PlayerTimePrinter> vis = injector.create<std::unique_ptr<fmapp::PlayerTimePrinter>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isWatchSet()) {
			std::shared_ptr<fmapp::SheetWatcher> vis = injector.create<std::unique_ptr<fmapp::SheetWatcher>>();
			loopVisitors.container.push_back(vis);
		}
		if (programOptionsPtr->isUdpSet()) {
			std::shared_ptr<fmapp::Funkfeuer> vis = injector.create<std::unique_ptr<fmapp::Funkfeuer>>();
			loopVisitors.container.push_back(vis);
		}

		auto program = injector.create<SheetPlayerProgram*>();
		sheetWatcherHandlers->container.push_back(program);
		program->prepareEnvironment();
		auto result = program->execute();
		sheetWatcherHandlers->container.clear();
		return result;
	} catch (const fm::Exception &ex) {
		std::cerr << ex.toString() << std::endl;
		return 1;
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	
}