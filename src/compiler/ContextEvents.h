#ifndef COMPILER_CONTEXT_EVENTS_HPP
#define COMPILER_CONTEXT_EVENTS_HPP

#include "sheet/Event.h"
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include "sheet/ChordDef.h"
#include <fm/common.hpp>
#include "forward.hpp"
#include "timeInfo.h"

namespace sheet {
    namespace compiler {
		struct NoteEvent {
			fm::Pitch pitch = 0;
			PitchDef::Octave octave = 0;
			fm::Ticks duration = 0;
			fm::Ticks absPosition = 0;
		};
    }
}

#endif