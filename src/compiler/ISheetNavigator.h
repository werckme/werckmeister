#ifndef ISHEET_NAVIGATOR_H
#define ISHEET_NAVIGATOR_H

#include "forward.hpp"
#include <memory>

namespace sheet {
	struct Voice;
	namespace compiler {
		/// Is responsible for solving navigation commands such as mark/jump etc.
		class ISheetNavigator {
		public:	
			virtual void processNavigation(Voice&) = 0;
		};
		typedef std::shared_ptr<ISheetNavigator> ISheetNavigatorPtr;
	}
}

#endif