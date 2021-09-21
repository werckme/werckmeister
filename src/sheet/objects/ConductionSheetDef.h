#ifndef SHEET_CONDUCTION_SHEET_H
#define SHEET_CONDUCTION_SHEET_H

#include "ASheetObjectWithSourceInfo.h"
#include <fm/common.hpp>
#include <vector>
#include <sheet/PitchDef.h>

namespace sheet {
	struct ConductionSelector :  public ASheetObjectWithSourceInfo {
		struct ArgumentValue {
			fm::Ticks numberValue = 0;
			PitchDef pitch;
			fm::String name;
		};
		typedef std::vector<ArgumentValue> Arguments; 
		fm::String type;
		Arguments arguments; 
	};

	struct ConductionRule : public ASheetObjectWithSourceInfo {
		typedef std::vector<ConductionSelector> Selectors;
		struct Declaration : public ASheetObjectWithSourceInfo {
			enum OperationType {
				OperationUnknown,
				OperationAdd,
				OperationSubstract,
				OperationSet,
				OperationFollowUpAdd,
				OperationFollowUpSubstract
			};
			enum ValueUnit {
				UnitAbsolute,
				UnitPercent	
			};
			fm::String property = "";
			OperationType operation = OperationUnknown;
			ValueUnit unit = UnitAbsolute;
			double value = 0;
		};
		typedef std::vector<Declaration> Declarations;
		Selectors selectors;
		Declarations declarations;
	};

	struct ConductionSheetDef : public ASheetObjectWithSourceInfo {
		typedef std::vector<ConductionRule> Rules;
		Rules rules;
	};

}

#endif