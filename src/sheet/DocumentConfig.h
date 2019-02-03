#ifndef DOCUMENT_CONFIG_HPP
#define DOCUMENT_CONFIG_HPP

#include <vector>
#include <fm/common.hpp>

namespace sheet {
	struct DocumentInfo {
		typedef std::vector<fm::String> Args;
		fm::String name;
		Args args;
	};	
	struct DocumentConfig {
		typedef fm::String Using;
		typedef std::vector<Using> Usings;
		typedef std::vector<DocumentInfo> DocumentInfos;
		Usings usings;
		DocumentInfos documentInfos;
	};
}

#endif