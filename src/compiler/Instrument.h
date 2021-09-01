#ifndef COMPILER_MIDI_INSTRUMENT_H
#define COMPILER_MIDI_INSTRUMENT_H

#include <fm/common.hpp>
#include <forward.hpp>
#include <fm/units.hpp>
#include "compiler/voicings/VoicingStrategy.h"
#include <unordered_map>
#include <list>

namespace sheet {
    namespace compiler {
		class IContext;
		struct AInstrumentDef : std::enable_shared_from_this<AInstrumentDef> {
			int id = 0;
			fm::String uname;
			double volume = 100.0;
			double pan = 50.0;
			VoicingStrategyPtr voicingStrategy;
			typedef std::list<AModificationPtr> Modifications;
			Modifications modifications;			
			typedef std::unordered_map<fm::Expression, double> VelocityOverride;
			VelocityOverride velocityOverride;
			virtual ~AInstrumentDef() = default;
			virtual void setToContext(IContext *context) = 0;
		protected:
			template<class TImpl>
			std::shared_ptr<TImpl> getThis() { return std::dynamic_pointer_cast<TImpl>(shared_from_this()); }
		};
		typedef std::shared_ptr<AInstrumentDef> AInstrumentDefPtr;
        struct MidiInstrumentDef : public AInstrumentDef {
			fm::String deviceName;
			int channel = 0;
			int cc = 0;
			int pc = 0;
			virtual ~MidiInstrumentDef() = default;
			virtual void setToContext(IContext* context) override;
		};

		struct InstrumentSectionDef : public AInstrumentDef {
            int id = 0;
			std::list<fm::String> instruments;
			virtual ~InstrumentSectionDef() = default;
			virtual void setToContext(IContext* context) override;
		};
    }
}
#endif