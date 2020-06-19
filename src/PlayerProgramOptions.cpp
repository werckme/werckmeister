#include "PlayerProgramOptions.h"

#ifdef WIN32
#define SIGINT_WORKAROUND
#endif

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_VERSION "version"
#define ARG_VERBOSE "verbose"
#define ARG_LIST "list"
#define ARG_LOOP "loop"
#define ARG_BEGIN "begin"
#define ARG_WATCH "watch"
#define ARG_UDP "funkfeuer"
#define ARG_NOSTDOUT "notime"
#define ARG_INFO "info"
#define ARG_PRINT_EVENTINFOS_JSON "print-events"
#define ARG_WIN32_SIGINT_WORKAROUND "win32-sigint-workaround"

void PlayerProgramOptions::parseProgrammArgs(size_t argc, const char **argv)
{
    namespace po = boost::program_options;
    optionsDescription.add_options()
        (ARG_HELP, "produce help message")
        (ARG_INPUT, po::value<std::string>(), "input file")
        (ARG_LIST, "list midi devices")
        (ARG_LOOP, "activates playback loop")
        (ARG_BEGIN, po::value<double>(), "start postition in quarter notes. E.g.: 1.2")
        (ARG_WATCH, "checks the input file for changes and recompiles if any")
        (ARG_UDP, po::value<std::string>(), "activates an udp sender, which sends sheet info periodically to to given host")
        (ARG_NOSTDOUT, "disable printing time on stdout")
        (ARG_INFO, "prints sheet info as json")
        (ARG_PRINT_EVENTINFOS_JSON, "prints the sheet events as json")
        (ARG_VERSION, "prints the werckmeister version")
        (ARG_VERBOSE, "prints (debug) informations to the output")
#ifdef SIGINT_WORKAROUND
        (ARG_WIN32_SIGINT_WORKAROUND, "uses a ipc workaround for the lack of a proper SIGINT signal handling in windows. \
(an ipc handler will be created before the player starts. If the handler will receive a ipc message, called by a separate program, the player will be stopped.)")

#endif
    ;
    po::positional_options_description p;
    p.add(ARG_INPUT, -1);
    po::store(po::command_line_parser(argc, argv).
        options(optionsDescription).positional(p).run(), variables);
    po::notify(variables);
}

bool PlayerProgramOptions::isListDevicesSet() const {
    return !!variables.count(ARG_LIST);
}

bool PlayerProgramOptions::isLoopSet() const {
    return !!variables.count(ARG_LOOP);
}

bool PlayerProgramOptions::isBeginSet() const {
    return customBeginValue > 0 || !!variables.count(ARG_BEGIN);
}

#include <iostream> 

double PlayerProgramOptions::getBegin() const {
    if (customBeginValue > 0) {
        return customBeginValue;
    }
    return variables[ARG_BEGIN].as<double>();
}

void PlayerProgramOptions::setBegin(double ticks) 
{
    customBeginValue = ticks;
}

bool PlayerProgramOptions::isWatchSet() const {
    return !!variables.count(ARG_WATCH);
}

bool PlayerProgramOptions::isUdpSet() const {
    return !!variables.count(ARG_UDP);
}

fm::String PlayerProgramOptions::getUdpHostname() const {
    return variables[ARG_UDP].as<std::string>();
}

bool PlayerProgramOptions::isNoTimePrintSet() const {
    return !!variables.count(ARG_NOSTDOUT);
}

bool PlayerProgramOptions::isDocumentInfoJSONSet() const {
    return !!variables.count(ARG_INFO);
}

bool PlayerProgramOptions::isEventInfosJSONSet() const {
    return !!variables.count(ARG_PRINT_EVENTINFOS_JSON);
}

bool PlayerProgramOptions::isSigintWorkaroundSet() const {
    return !!variables.count(ARG_WIN32_SIGINT_WORKAROUND);
}