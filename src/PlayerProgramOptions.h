#ifndef PLAYERPROGRAMOPTIONS_HPP
#define PLAYERPROGRAMOPTIONS_HPP

#include <fm/common.hpp>
#include <boost/program_options.hpp>
#include <IPlayerProgramOptions.h>
#include <CompilerProgramOptions.h>

struct PlayerProgramOptions : public CompilerProgramOptions, public IPlayerProgramOptions {
	virtual ~PlayerProgramOptions() = default;
	virtual void parseProgrammArgs(size_t argc, const char **argv) override;

	virtual bool isListDevicesSet() const override;
	virtual bool isLoopSet() const override;
	virtual bool isBeginSet() const override;
	virtual double getBegin() const override;
	virtual bool isEndSet() const override;
	virtual double getEnd() const override;
	virtual void setResumeAtPosition(double quarters) override { resumeAtPosition = quarters; }
	virtual double getResumeAtPosition() override { return resumeAtPosition; }
	virtual bool isWatchSet() const override;
	virtual bool isUdpSet() const override;
	virtual fm::String getUdpHostname() const override;
	virtual bool isNoTimePrintSet() const override;
	virtual bool isSigintWorkaroundSet() const override;
	virtual bool isJsonDocInfoMode() const override;

	// compileroptions we don't need
	virtual bool isJsonModeSet() const override { return false; }
	virtual bool isOutputSet() const override { return false; }
	virtual bool isNoMetaSet() const override { return false; }
	virtual fm::String getOutput() const override { return ""; }
private:
	double resumeAtPosition = 0;
};

#endif