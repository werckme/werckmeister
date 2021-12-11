#ifndef GROUPED_H
#define GROUPED_H

#include "Event.h"

namespace documentModel {
	struct Grouped : public Event {
		Grouped() {
			this->type = Group;
		}
	};
}

#endif