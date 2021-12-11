#ifndef COMPILER_CONTEXT_EVENTS_HPP
#define COMPILER_CONTEXT_EVENTS_HPP

#include <sheet/objects/Event.h>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <sheet/objects/ChordDef.h>
#include <com/common.hpp>
#include "forward.hpp"
#include "timeInfo.h"

namespace sheet {
    namespace compiler {
		struct NoteEvent {
			com::Pitch pitch = 0;
			PitchDef::Octave octave = 0;
			com::Ticks duration = 0;
			com::Ticks absPosition = 0;
		};
    }
}

#endif