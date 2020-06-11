#ifndef ICOMPILER_PREPROCESSOR_H
#define ICOMPILER_PREPROCESSOR_H

#include <fm/common.hpp>

namespace sheet {
	struct Track;
	namespace compiler {
        /**
         * sets implicite duration to every event with duration = 0.
		 * sets x shortcut e.g.: c1 x x x
		 * set implicit EOB
         */
		class IPreprocessor {
		public:
			virtual void process(Track &track) = 0;
		};
	}
}

#endif