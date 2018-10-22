#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <vector>

namespace sheet {

	namespace events {

		struct AEvent {
			virtual ~AEvent() = default;
		};

		struct Note : public AEvent {

		};

		struct Accord : public AEvent {

		};

		struct MetaEvent : public AEvent {

		};
	
	}


}

#endif