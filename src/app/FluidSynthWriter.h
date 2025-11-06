
#pragma once

#include "FluidSynthWrapper.h"

namespace app
{
	class FluidSynthWriter : public FluidSynth
	{
	public:
		virtual ~FluidSynthWriter() = default;
		virtual void initSynth(const std::string soundFondPath) override;
		bool addEvent(const com::midi::Event& event);
	protected:
        virtual std::string findFluidSynthLibraryPath() const override;
	};
	typedef std::shared_ptr<FluidSynthWriter> FluidSynthWriterPtr;
}