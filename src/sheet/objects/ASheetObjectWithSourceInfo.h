#ifndef ASHEET_OBJECT_WITH_SOURCE_INFO_H
#define ASHEET_OBJECT_WITH_SOURCE_INFO_H

#include <climits>
#include "ASheetObject.h"

namespace sheet {

    class ASheetObjectWithSourceInfo : public ASheetObject {
    public:
		enum {
			UndefinedSource = 0,
			UndefinedPosition = INT_MAX
		};
		typedef unsigned int SourceId;
		SourceId sourceId = UndefinedSource;
		/**
		 * where in the source is begins this object 
		 */
		unsigned int sourcePositionBegin = UndefinedPosition;
		unsigned int sourcePositionEnd = UndefinedPosition;
    };
}

#endif