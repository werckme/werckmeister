#ifndef COMPILER_MIDI_INSTRUMENT_H
#define COMPILER_MIDI_INSTRUMENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include "compiler/voicings/VoicingStrategy.h"
#include <unordered_map>

namespace sheet {
    namespace compiler {
		struct AInstrumentDef {
			fm::String uname;
			VoicingStrategyPtr voicingStrategy;
		};
        struct MidiInstrumentDef : AInstrumentDef {
			typedef std::unordered_map<fm::Expression, int> ExpressionOverride;
			ExpressionOverride expressionOverride;
            int id = 0;
			fm::String deviceName;
			int channel = 0;
			int cc = 0;
			int pc = 0;
			int volume = 100;
			int pan = 50;
		};
    }
}
#endif