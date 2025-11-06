
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
		void render(int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr);
	protected:
        virtual std::string findFluidSynthLibraryPath() const override;
	};
	typedef std::shared_ptr<FluidSynthWriter> FluidSynthWriterPtr;
}