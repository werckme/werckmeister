#ifndef AMIDI_PROVIDER_H
#define AMIDI_PROVIDER_H

#include <fm/units.hpp>

namespace fmapp {
	class AMidiProvider {
	public:
		virtual ~AMidiProvider() = default;
		virtual void reset() {}
		virtual void seek(fm::Ticks ticks) {}
	};
}

#endif