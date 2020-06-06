#ifndef GROUPED_H
#define GROUPED_H

#include "Event.h"

namespace sheet {
	struct Grouped : public Event {
		Grouped() {
			this->type = Group;
		}
	};
}

#endif