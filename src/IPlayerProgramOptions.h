#ifndef IPLAYERPROGRAMOPTIONS_HPP
#define IPLAYERPROGRAMOPTIONS_HPP

#include "ICompilerProgramOptions.h"

struct IPlayerProgramOptions : public virtual ICompilerProgramOptions {
	virtual bool isListDevicesSet() const = 0;
	virtual bool isLoopSet() const = 0;
	virtual bool isBeginSet() const = 0;
	virtual int getBegin() const = 0;
	virtual bool isWatchSet() const = 0;
	virtual bool isUdpSet() const = 0;
	virtual fm::String getUdpHostname() const = 0;
	virtual bool isNoTimePrintSet() const = 0;
	virtual bool isDocumentInfoJSONSet() const = 0;
	virtual bool isEventInfosJSONSet() const = 0;
	virtual bool isSigintWorkaroundSet() const = 0;
};

typedef std::shared_ptr<ICompilerProgramOptions> IPlayerProgramOptionsPtr;

#endif