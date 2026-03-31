#pragma once

#include <com/common.hpp>
#include <boost/program_options.hpp>
#include <ICompilerProgramOptions.h>

struct CompilerProgramOptions : public virtual ICompilerProgramOptions
{
	CompilerProgramOptions() : optionsDescription("Allowed options") {}
	virtual ~CompilerProgramOptions() = default;
	virtual bool isHelpSet() const override;
	virtual bool isInputSet() const override;
	virtual bool isVerboseSet() const override;
	virtual bool isDebugSet() const override;
	virtual com::String getInput() const override;
	virtual bool isOutputSet() const override;
	virtual bool isNoMetaSet() const override;
	virtual com::String getOutput() const override;
	virtual bool isJsonModeSet() const override;
	virtual bool isJsonDocInfoMode() const override;
	virtual bool isVersionSet() const override;
	virtual void parseProgrammArgs(size_t argc, const char **argv);
	virtual void printHelpText(std::ostream &os) override;
	virtual bool isBeginSet() const override;
	virtual double getBegin() const override;
	virtual bool isEndSet() const override;
	virtual double getEnd() const override;
	virtual bool isJsonDebugInfoMode() const override;
	typedef boost::program_options::variables_map Variables;
	typedef boost::program_options::options_description OptionsDescription;
	OptionsDescription optionsDescription;
	Variables variables;
};
