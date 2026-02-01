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
#define ARG_WATCH "watch"
#define ARG_UDP "funkfeuer"
#define ARG_NOSTDOUT "notime"
#define ARG_INFO "info"
#define ARG_WIN32_SIGINT_WORKAROUND "win32-sigint-workaround"
#define ARG_DEBUG "debug"
#define ARG_BEGIN "begin"
#define ARG_END "end"
#define ARG_SCRIPT "script"

void PlayerProgramOptions::parseProgrammArgs(size_t argc, const char **argv)
{
    namespace po = boost::program_options;
    optionsDescription.add_options()
        (ARG_HELP, "produce help message")
        (ARG_INPUT, po::value<std::string>(), "input file")
        (ARG_LIST, "list midi devices")
        (ARG_LOOP, "activates playback loop")
        (ARG_WATCH, "checks the input file for changes and recompiles if any")
        (ARG_UDP, po::value<std::string>(), "activates an udp sender, which sends documentModel info periodically to to given host")
        (ARG_NOSTDOUT, "disable printing time on stdout")
        (ARG_INFO, "prints documentModel info as json")
        (ARG_VERSION, "prints the werckmeister version")
        (ARG_VERBOSE, "prints informations to the output")
        (ARG_DEBUG, "prints debug informations to the output")
        (ARG_BEGIN, po::value<double>(), "start postition in quarter notes. E.g.: 1.2")
        (ARG_END, po::value<double>(), "end postition in quarter notes. E.g.: 1.2")
        (ARG_SCRIPT, po::value<std::string>(), "path to a lua script, which will be performed during playback")

#ifdef SIGINT_WORKAROUND
        (ARG_WIN32_SIGINT_WORKAROUND, "uses a ipc workaround for the lack of a proper SIGINT signal handling in windows. \
(an ipc handler will be created before the player starts. If the handler will receive a ipc message, called by a separate program, the player will be stopped.)")

#endif
        ;
    po::positional_options_description p;
    p.add(ARG_INPUT, -1);
    po::store(po::command_line_parser((int)argc, argv).options(optionsDescription).positional(p).run(), variables);
    po::notify(variables);
}

bool PlayerProgramOptions::isJsonDocInfoMode() const
{
    return !!variables.count(ARG_INFO);
}

bool PlayerProgramOptions::isListDevicesSet() const
{
    return !!variables.count(ARG_LIST);
}

bool PlayerProgramOptions::isLoopSet() const
{
    return !!variables.count(ARG_LOOP);
}

bool PlayerProgramOptions::isWatchSet() const
{
    return !!variables.count(ARG_WATCH);
}

bool PlayerProgramOptions::isUdpSet() const
{
    return !!variables.count(ARG_UDP);
}

com::String PlayerProgramOptions::getUdpHostname() const
{
    return variables[ARG_UDP].as<std::string>();
}

bool PlayerProgramOptions::isNoTimePrintSet() const
{
    return !!variables.count(ARG_NOSTDOUT);
}

bool PlayerProgramOptions::isSigintWorkaroundSet() const
{
    return !!variables.count(ARG_WIN32_SIGINT_WORKAROUND);
}

bool PlayerProgramOptions::isScriptSet() const
{
    return !!variables.count(ARG_SCRIPT);
}

com::String PlayerProgramOptions::script() const
{
    return variables[ARG_SCRIPT].as<std::string>();
}
