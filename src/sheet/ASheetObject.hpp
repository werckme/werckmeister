#ifndef ASHEET_OBJECT_H
#define ASHEET_OBJECT_H

#include <climits>

namespace sheet {
    class ASheetObject {
    };

    class ASheetObjectWithSourceInfo {
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
    };

    class ParserSourceInfo : public ASheetObjectWithSourceInfo {};
}

#endif