#ifndef SHEET_NAVIGATOR_H
#define SHEET_NAVIGATOR_H

#include "ISheetNavigator.h"

namespace sheet {
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

#endif