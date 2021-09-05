#ifndef SHEET_CONDUCTION_SHEET_H
#define SHEET_CONDUCTION_SHEET_H

#include "ASheetObjectWithSourceInfo.h"
#include <fm/common.hpp>
#include <vector>
#include <sheet/PitchDef.h>

namespace sheet {
	struct ConductionSelector :  public ASheetObjectWithSourceInfo {
		struct ArgumentValue {
			fm::Ticks tickValue = 0;
			PitchDef pitch;
		};
		typedef std::vector<ArgumentValue> Arguments; 
		enum Type { TypeUnkown, TypePosition, TypePitch };
		Type type = TypeUnkown;
		Arguments arguments; 
	};

	struct ConductionRule : public ASheetObjectWithSourceInfo {
		typedef std::vector<ConductionSelector> Selectors;
		Selectors selectors;
	};

	struct ConductionSheetDef : public ASheetObjectWithSourceInfo {
		typedef std::vector<ConductionRule> Rules;
		Rules rules;
	};

}

#endif