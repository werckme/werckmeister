#ifndef COMPILER_PREPROCESSOR_H
#define COMPILER_PREPROCESSOR_H

#include <fm/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>

namespace sheet {
	struct Track;
	namespace compiler {
        /**
         * sets implicite duration to every event with duration = 0.
		 * sets x shortcut e.g.: c1 x x x
		 * set implicit EOB
         */
		class Preprocessor : public IPreprocessor {
		private:
			IContext 			*_context;
			ASheetEventRenderer *_renderer;
		public:
			Preprocessor(IContext *context, ASheetEventRenderer *renderer)
				: _context(context), _renderer(renderer)
			{}
			Preprocessor(const Preprocessor&) = delete;
			Preprocessor & operator=(const Preprocessor&) = delete;
			virtual void process(Track &track);
			virtual void preprocessSheetTrack(Track &sheetTrack);

		};
	}
}

#endif