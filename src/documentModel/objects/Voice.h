#pragma once

#include <com/common.hpp>
#include <vector>
#include "Event.h"
#include "ASheetObject.h"
#include <documentModel/Argument.h>
#include "Event.h"

namespace documentModel {

	struct Voice : public ASheetObject {
		typedef std::vector<Event> Events;
		Events events;
	};

}

