#ifndef IPLAYERPROGRAMOPTIONS_HPP
#define IPLAYERPROGRAMOPTIONS_HPP

#include "ICompilerProgramOptions.h"

struct IPlayerProgramOptions : public virtual ICompilerProgramOptions {
	virtual bool isListDevicesSet() const = 0;
	virtual bool isLoopSet() const = 0;
	virtual bool isWatchSet() const = 0;
	virtual bool isUdpSet() const = 0;
	virtual com::String getUdpHostname() const = 0;
	virtual bool isNoTimePrintSet() const = 0;
	virtual bool isSigintWorkaroundSet() const = 0;
	virtual void setResumeAtPosition(double quarters) = 0;
	virtual double getResumeAtPosition() = 0;
};

typedef std::shared_ptr<IPlayerProgramOptions> IPlayerProgramOptionsPtr;

#endif