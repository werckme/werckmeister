#ifndef ICOMPILER_H
#define ICOMPILER_H

#include <fm/common.hpp>
#include "forward.hpp"

namespace sheet {
	struct Event;
	namespace compiler {
		class ICompiler {
		public:
			virtual void compile(DocumentPtr document) = 0;
		};
	}
}

#endif