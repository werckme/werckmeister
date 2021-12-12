#pragma once

#include <com/common.hpp>
#include <ostream>
#include <memory>

struct ICompilerProgramOptions
{
	virtual bool isHelpSet() const = 0;
	virtual bool isInputSet() const = 0;
	virtual com::String getInput() const = 0;
	virtual bool isOutputSet() const = 0;
	virtual bool isNoMetaSet() const = 0;
	virtual com::String getOutput() const = 0;
	virtual bool isJsonModeSet() const = 0;
	virtual bool isJsonDocInfoMode() const = 0;
	virtual bool isVersionSet() const = 0;
	virtual void printHelpText(std::ostream &os) = 0;
	virtual bool isVerboseSet() const = 0;
	virtual bool isDebugSet() const = 0;
	virtual bool isBeginSet() const = 0;
	virtual double getBegin() const = 0;
	virtual bool isEndSet() const = 0;
	virtual double getEnd() const = 0;
	virtual ~ICompilerProgramOptions() = default;
};

typedef std::shared_ptr<ICompilerProgramOptions> ICompilerProgramOptionsPtr;
