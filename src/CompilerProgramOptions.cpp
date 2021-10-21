#include "CompilerProgramOptions.h"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_OUTPUT "output"
#define ARG_MODE "mode"
#define ARG_NOMETA "nometa"
#define ARG_VERSION "version"
#define ARG_VERBOSE "verbose"
#define ARG_DEBUG "debug"
#define ARG_BEGIN "begin"
#define ARG_END "end"

void CompilerProgramOptions::parseProgrammArgs(size_t argc, const char **argv)
{
    namespace po = boost::program_options;
    optionsDescription.add_options()
        (ARG_HELP, "produce help message")
        (ARG_INPUT, po::value<std::string>(), "input file")
        (ARG_OUTPUT, po::value<std::string>(), "output file")
        (ARG_MODE, po::value<std::string>(), "mode: normal | json | validate; in JSON mode the input and output will be implemented using JSON strings. The input JSON has to be Base64 encoded. \
Validate mode checks for errors and returns the validation result as json object.")
        (ARG_NOMETA, "dosen't render midi meta events like track name or tempo")
        (ARG_VERSION, "prints the werckmeister version")
        (ARG_VERBOSE, "prints further informations to the output")
        (ARG_DEBUG, "prints debug informations to the output")
        (ARG_BEGIN, po::value<double>(), "start postition in quarter notes. E.g.: 1.2")
        (ARG_END,   po::value<double>(), "end postition in quarter notes. E.g.: 1.2")
        ;
    po::positional_options_description p;
    p.add(ARG_INPUT, -1);
    po::store(po::command_line_parser((int)argc, argv).
        options(optionsDescription).positional(p).run(), variables);
    po::notify(variables);
}

void CompilerProgramOptions::printHelpText(std::ostream &os)
{
    os << optionsDescription;
}

bool CompilerProgramOptions::isVerboseSet() const
{
    return !!variables.count(ARG_VERBOSE);
}

bool CompilerProgramOptions::isDebugSet() const
{
    return !!variables.count(ARG_DEBUG);
}

bool CompilerProgramOptions::isHelpSet() const 
{ 
    return !!variables.count(ARG_HELP); 
}

bool CompilerProgramOptions::isInputSet() const 
{ 
    return !!variables.count(ARG_INPUT); 
}

fm::String CompilerProgramOptions::getInput() const 
{ 
    return variables[ARG_INPUT].as<std::string>(); 
}

bool CompilerProgramOptions::isOutputSet() const 
{ 
    return !!variables.count(ARG_OUTPUT); 
}

bool CompilerProgramOptions::isNoMetaSet() const 
{ 
    return !!variables.count(ARG_NOMETA); 
}

fm::String CompilerProgramOptions::getOutput() const 
{ 
    return variables[ARG_OUTPUT].as<std::string>(); 
}

bool CompilerProgramOptions::isJsonModeSet() const 
{
    if (variables.count(ARG_MODE) == 0) {
        return false;
    }
    return variables[ARG_MODE].as<std::string>() == "json";
}

bool CompilerProgramOptions::isJsonDocInfoMode() const 
{
    if (variables.count(ARG_MODE) == 0) {
        return false;
    }
    return variables[ARG_MODE].as<std::string>() == "validate";
}

bool CompilerProgramOptions::isVersionSet() const 
{ 
    return !!variables.count(ARG_VERSION); 
}

bool CompilerProgramOptions::isBeginSet() const {
    return !!variables.count(ARG_BEGIN);
}

bool CompilerProgramOptions::isEndSet() const {
    return !!variables.count(ARG_END);
}

double CompilerProgramOptions::getBegin() const {
    return variables[ARG_BEGIN].as<double>();
}

double CompilerProgramOptions::getEnd() const {
    return variables[ARG_END].as<double>();
}