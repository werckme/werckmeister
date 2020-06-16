#ifndef ICOMPILERPROGRAMOPTIONS_HPP
#define ICOMPILERPROGRAMOPTIONS_HPP

#include <fm/common.hpp>
#include <ostream>
#include <memory>

struct ICompilerProgramOptions {
	virtual bool isHelpSet() const = 0;
	virtual bool isInputSet() const = 0;
	virtual fm::String getInput() const = 0;
	virtual bool isOutputSet() const = 0;
	virtual bool isNoMetaSet() const = 0;
	virtual fm::String getOutput() const = 0;
	virtual bool isJsonModeSet() const = 0;
	virtual bool isValidateMode() const = 0;
	virtual bool isVersionSet() const = 0;
	virtual void printHelpText(std::ostream &os) = 0;
	virtual bool isVerboseSet() const = 0;
	virtual ~ICompilerProgramOptions() = default;
};

typedef std::shared_ptr<ICompilerProgramOptions> ICompilerProgramOptionsPtr;

#endif