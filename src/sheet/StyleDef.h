#ifndef STYLE_DEF_H
#define STYLE_DEF_H

#include <fm/common.hpp>
#include "Track.h"
#include <vector>

namespace sheet {

	struct Section {
		typedef std::vector<Track> Tracks;
		fm::String name;
		Tracks tracks;
	};

	struct StyleDef {
		typedef std::vector<Section> Sections;
		Sections sections;
	};

}

#endif