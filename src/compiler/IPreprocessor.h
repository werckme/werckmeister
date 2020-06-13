#ifndef ICOMPILER_PREPROCESSOR_H
#define ICOMPILER_PREPROCESSOR_H

#include <fm/common.hpp>
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <memory>
namespace sheet {
	struct Track;
	namespace compiler {
		class IPreprocessor {
		public:
			/**
			 * resolves implicite duration to every event time consuming with duration = 0.
			 * resolves x shortcut e.g.: c1 x x x -> c1 c1 c1 c1
			 * set implicit EOB
			 */
			virtual void process(Track &track) = 0;
			/**
			 * determines the duration of every chord event, such as:
			 * C D | E -> C(2) D(2) | E(4)
			 * set implicit EOB
			 */			
			virtual void preprocessSheetTrack(Track &sheetTrack) = 0;
			virtual void prepareTemplateDefinitions(DocumentPtr document) = 0;
		};
		typedef std::shared_ptr<IPreprocessor> IPreprocessorPtr;
	}
}

#endif