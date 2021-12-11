#pragma once

#include "Event.h"

namespace documentModel {
	struct Grouped : public Event {
		Grouped() {
			this->type = Group;
		}
	};
}

