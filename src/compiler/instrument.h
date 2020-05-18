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

		struct AInstrumentDef {
			fm::String uname;
			int volume = 100;
			int pan = 50;
			VoicingStrategyPtr voicingStrategy;
			typedef std::list<AModificationPtr> Modifications;
			Modifications modifications;			
			typedef std::unordered_map<fm::Expression, int> VelocityOverride;
			VelocityOverride velocityOverride;
			virtual ~AInstrumentDef() = default;
		};

        struct MidiInstrumentDef : public AInstrumentDef {
            int id = 0;
			fm::String deviceName;
			int channel = 0;
			int cc = 0;
			int pc = 0;
			virtual ~MidiInstrumentDef() = default;
		};
    }
}
#endif