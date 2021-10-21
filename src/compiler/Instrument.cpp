#include "Instrument.h"
#include <compiler/context/MidiContext.h>
#include <compiler/SheetEventRenderer.h>
#include <compiler/modification/AModification.h>

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

		void MidiInstrumentDef::renderEvents(SheetEventRenderer *renderer, std::list<Event>& events)
		{
			auto ctx_ = renderer->context();
			auto meta = ctx_->voiceMetaData();
			for (auto mod : modifications) {
				mod->perform(ctx_, events);
			}
			for (const auto& event : events) {
				if (event.isPitchBend()) {
					renderer->renderPitchBendEvent(event);
				}
				else {
					renderer->renderEventPitches(event);
				}
			}
		}

		void InstrumentSectionDef::setToContext(IContext *ctx) 
		{
			auto midiContext = dynamic_cast<MidiContext*>(ctx);
			if (!midiContext) {
				throw std::runtime_error("expecting a midi context");
			}
			for (const auto& uname : instrumentNames) {
				if (uname == this->uname) {
					FM_THROW(Exception, "instrumentSection '" + this->uname + "' can not reference to it self");
				}
				auto instrumentDef = ctx->getInstrumentDef(uname);
				bool isInstrumentSection = std::dynamic_pointer_cast<InstrumentSectionDef>(instrumentDef) != nullptr;
				if (isInstrumentSection) {
					FM_THROW(Exception, "instrumentSection '" + this->uname + "' can not reference to another instrumentSection '" + uname +  "'");
				}
				if (!instrumentDef) {
					FM_THROW(Exception, "section instrument not found: " + uname);
				}
				_currentInstrument = instrumentDef;
				instrumentDef->setToContext(ctx);
			}
			_currentInstrument.reset();
			midiContext->setInstrument(getThis<InstrumentSectionDef>());
		}

		void InstrumentSectionDef::renderEvents(SheetEventRenderer* renderer, std::list<Event>& events)
		{
			auto ctx_ = renderer->context();
			VoiceMetaData voiceMetaCopy = *ctx_->voiceMetaData();
			for (const auto& uname : instrumentNames) {
				auto instrumentDef = ctx_->getInstrumentDef(uname);
				if (!instrumentDef) {
					FM_THROW(Exception, "section instrument not found: " + uname);
				}
				_currentInstrument = instrumentDef;
				std::list<Event> copy = events; // we like it original here
				auto contextMetaPtr = ctx_->voiceMetaData();
				*contextMetaPtr = voiceMetaCopy;
				instrumentDef->renderEvents(renderer, copy);
			}
			_currentInstrument.reset();
		}
	}
}