#ifndef ICONDUCTIONS_PERFORMER_H
#define ICONDUCTIONS_PERFORMER_H

#include <com/common.hpp>
#include <forward.hpp>
#include <memory>

namespace documentModel
{
	namespace conductor
	{
		class IConductionsPerformer
		{
		public:
			virtual void applyConductions() = 0;
		};
		typedef std::shared_ptr<IConductionsPerformer> IConductionsPerformerPtr;
	}
}

#endif