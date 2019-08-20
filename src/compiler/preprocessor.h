#ifndef COMPILER_PREPROCESSOR_H
#define COMPILER_PREPROCESSOR_H

#include <fm/common.hpp>

namespace sheet {
	struct Track;
	namespace compiler {
        /**
         * sets implicite duration to every event with duration = 0.
		 * sets x shortcut e.g.: c1 x x x
         */
		class Preprocessor {
		public:
			Preprocessor() = default;
			Preprocessor(const Preprocessor&) = delete;
			Preprocessor & operator=(const Preprocessor&) = delete;
			void process(Track &track);
		private:
		};
	}
}

#endif