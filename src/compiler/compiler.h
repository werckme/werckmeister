#ifndef COMPILER_H
#define COMPILER_H

#include <fm/common.hpp>


namespace sheet {
	namespace compiler {
		class Compiler {
		public:
			Compiler();
			Compiler(const Compiler&) = delete;
			Compiler & operator=(const Compiler&) = delete;
		};
	}
}

#endif