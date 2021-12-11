#ifndef DOCUMENT_CONFIG_HPP
#define DOCUMENT_CONFIG_HPP

#include <vector>
#include <com/common.hpp>

namespace sheet {
	struct DocumentUsing {
		typedef com::String Using;
		typedef std::vector<Using> Usings;
		Usings usings;
	};
}

#endif