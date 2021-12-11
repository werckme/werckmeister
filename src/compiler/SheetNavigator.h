#pragma once

#include "ISheetNavigator.h"

namespace documentModel {
	namespace compiler {
		/// Is responsible for solving navigation commands such as mark/jump etc.
		class SheetNavigator : public ISheetNavigator {
		public:	
			virtual void processNavigation(Voice&) override;
		private:
			void processJumps(Voice&);
			void processRepeats(Voice&);
		};
	}
}

