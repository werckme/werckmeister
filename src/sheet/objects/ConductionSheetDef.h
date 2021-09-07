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
		enum Type { TypeUnkown, TypeFromPosition, TypeToPosition, TypePitch, TypeAtBeat };
		Type type = TypeUnkown;
		Arguments arguments; 
	};

	struct ConductionRule : public ASheetObjectWithSourceInfo {
		typedef std::vector<ConductionSelector> Selectors;
		struct Declaration : public ASheetObjectWithSourceInfo {
			enum Property {
				PropertyUnknown,
				PropertyVelocity,
				PropertyTime
			};
			enum OperationType {
				OperationUnknown,
				OperationAdd,
				OperationSet
			};
			enum ValueUnit {
				UnitAbsolute,
				UnitPercent	
			};
			Property property = PropertyUnknown;
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