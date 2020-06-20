#ifndef PLAYERPROGRAMOPTIONS_HPP
#define PLAYERPROGRAMOPTIONS_HPP

#include <fm/common.hpp>
#include <boost/program_options.hpp>
#include <IPlayerProgramOptions.h>
#include <CompilerProgramOptions.h>

struct PlayerProgramOptions : public CompilerProgramOptions, public IPlayerProgramOptions {
	virtual ~PlayerProgramOptions() = default;
	virtual void parseProgrammArgs(size_t argc, const char **argv) override;

	virtual bool isListDevicesSet() const;
	virtual bool isLoopSet() const;
	virtual bool isBeginSet() const;
	virtual double getBegin() const;
	virtual bool isEndSet() const;
	virtual double getEnd() const;	
	virtual void setResumeAtPosition(double quarters) override { resumeAtPosition = quarters; }
	virtual double getResumeAtPosition() override { return resumeAtPosition; }
	virtual bool isWatchSet() const;
	virtual bool isUdpSet() const;
	virtual fm::String getUdpHostname() const;
	virtual bool isNoTimePrintSet() const;
	virtual bool isDocumentInfoJSONSet() const;
	virtual bool isEventInfosJSONSet() const;
	virtual bool isSigintWorkaroundSet() const;

	// compileroptions we don't need
	virtual bool isOutputSet() const override 		{ return false; }
	virtual bool isNoMetaSet() const override 		{ return false; }
	virtual fm::String getOutput() const override 	{ return ""; }
	virtual bool isJsonModeSet() const override		{ return false; }
	virtual bool isValidateMode() const override	{ return false; }
private:
	double resumeAtPosition = 0;
};

#endif