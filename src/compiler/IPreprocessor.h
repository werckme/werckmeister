#pragma once

#include <com/common.hpp>
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <memory>
namespace documentModel
{
	struct Track;
}
namespace compiler
{
	class IPreprocessor
	{
	public:
		/**
		 * for note event tracks:
		 *   resolves implicite duration to every event time consuming with duration = 0.
		 *   resolves x shortcut e.g.: c1 x x x -> c1 c1 c1 c1
		 *   determines total duration of tied events
		 *   set implicit EOB
		 * for chord tracks:
		 *   determines the duration of every chord event, such as:
		 *   C D | E -> C(2) D(2) | E(4)
		 * set implicit EOB
		 * processes navigation commands such as mark/jump
		 */
		virtual void preprocess(DocumentPtr document) = 0;
	};
	typedef std::shared_ptr<IPreprocessor> IPreprocessorPtr;
}
