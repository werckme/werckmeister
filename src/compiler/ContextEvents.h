#pragma once

#include <documentModel/objects/Event.h>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <documentModel/objects/ChordDef.h>
#include <com/common.hpp>
#include "forward.hpp"
#include "timeInfo.h"

namespace documentModel {
    namespace compiler {
		struct NoteEvent {
			com::Pitch pitch = 0;
			PitchDef::Octave octave = 0;
			com::Ticks duration = 0;
			com::Ticks absPosition = 0;
		};
    }
}

