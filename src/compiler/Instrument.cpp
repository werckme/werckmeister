#include "Instrument.h"
#include <compiler/context/MidiContext.h>

namespace sheet 
{
    namespace compiler 
	{
		void MidiInstrumentDef::setToContext(IContext *ctx) 
		{
			auto midiContext = dynamic_cast<MidiContext*>(ctx);
			if (!midiContext) {
				throw std::runtime_error("expecting a midi context");
			}
			midiContext->setInstrument(getThis<MidiInstrumentDef>());
		}
		void InstrumentSectionDef::setToContext(IContext *ctx) 
		{
			auto midiContext = dynamic_cast<MidiContext*>(ctx);
			if (!midiContext) {
				throw std::runtime_error("expecting a midi context");
			}
			midiContext->setInstrument(getThis<InstrumentSectionDef>());
		}
	}
}