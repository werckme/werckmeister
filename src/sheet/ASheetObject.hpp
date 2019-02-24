#ifndef ASHEET_OBJECT_H
#define ASHEET_OBJECT_H



namespace sheet {
    class ASheetObject {
    };

    class ASheetObjectWithSourceInfo {
    public:
		enum {
			UndefinedSource = 0
		};
        typedef unsigned int SourceId;
        SourceId sourceId = UndefinedSource;
		/**
		 * where in the source is begins this object 
		 */
		unsigned int sourcePositionBegin = 0;
    };
}

#endif