#ifndef COMPILERPROGRAMOPTIONS_HPP
#define COMPILERPROGRAMOPTIONS_HPP

#include <com/common.hpp>
#include <boost/program_options.hpp>
#include <ICompilerProgramOptions.h>

struct CompilerProgramOptions : public virtual ICompilerProgramOptions {
	CompilerProgramOptions() : optionsDescription("Allowed options") {}
	virtual ~CompilerProgramOptions() = default;
	virtual bool isHelpSet() const;
	virtual bool isInputSet() const;
	virtual bool isVerboseSet() const;
	virtual bool isDebugSet() const;
	virtual com::String getInput() const;
	virtual bool isOutputSet() const;
	virtual bool isNoMetaSet() const;
	virtual com::String getOutput() const;
	virtual bool isJsonModeSet() const;
	virtual bool isJsonDocInfoMode() const;
	virtual bool isVersionSet() const;
	virtual void parseProgrammArgs(size_t argc, const char **argv);
	virtual void printHelpText(std::ostream &os);
	virtual bool isBeginSet() const override;
	virtual double getBegin() const override;
	virtual bool isEndSet() const override;
	virtual double getEnd() const override;	
	typedef boost::program_options::variables_map Variables;
	typedef boost::program_options::options_description OptionsDescription;
	OptionsDescription optionsDescription;
	Variables variables;
};

#endif