#ifndef DOCUMENT_CONFIG_HPP
#define DOCUMENT_CONFIG_HPP

#include <vector>
#include <fm/common.hpp>

namespace sheet {
	struct DocumentConfig {
		typedef fm::String Using;
		typedef std::vector<Using> Usings;
		Usings usings;
	};
}

#endif