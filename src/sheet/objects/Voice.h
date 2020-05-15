#ifndef VOICE_H
#define VOICE_H

#include <fm/common.hpp>
#include <vector>
#include "Event.h"
#include "ASheetObject.h"
#include <sheet/Argument.h>
#include "Event.h"

namespace sheet {

	struct Voice : public ASheetObject {
		typedef std::vector<Event> Events;
		Events events;
	};

}

#endif