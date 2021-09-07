#ifndef ICONDUCTIONS_PERFORMER_H
#define ICONDUCTIONS_PERFORMER_H

#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>

namespace sheet
{
	namespace conductor
	{
		class IConductionsPerformer
		{
		public:
			virtual void applyConductions(DocumentPtr document) = 0;
		};
		typedef std::shared_ptr<IConductionsPerformer> IConductionsPerformerPtr;
	}
}

#endif