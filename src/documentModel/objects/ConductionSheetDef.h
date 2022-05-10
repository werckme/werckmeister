#pragma once

#include "ASheetObjectWithSourceInfo.h"
#include <com/common.hpp>
#include <vector>
#include <documentModel/PitchDef.h>

namespace documentModel
{
	struct ConductionSelector : public ASheetObjectWithSourceInfo
	{
		struct ArgumentValue
		{
			enum ValueContext { Unspecified, CueReference };
			com::Ticks numberValue = 0;
			PitchDef pitch;
			com::String name;
			ValueContext valueContext = Unspecified;
		};
		typedef std::vector<ArgumentValue> Arguments;
		com::String type;
		Arguments arguments;
	};

	struct ConductionRule : public ASheetObjectWithSourceInfo
	{
		typedef std::vector<ConductionSelector> Selectors;
		typedef std::vector<Selectors> SelectorsSet;
		struct Declaration : public ASheetObjectWithSourceInfo
		{
			enum OperationType
			{
				OperationUnknown,
				OperationAdd,
				OperationSubstract,
				OperationSet,
				OperationFollowUpAdd,
				OperationFollowUpSubstract
			};
			enum ValueUnit
			{
				UnitAbsolute,
				UnitPercent
			};
			com::String property = "";
			OperationType operation = OperationUnknown;
			ValueUnit unit = UnitAbsolute;
			double value = 0;
		};
		typedef std::vector<Declaration> Declarations;
		Selectors selectors;
		SelectorsSet selectorsSet;
		Declarations declarations;
	};

	struct ConductionSheetDef : public ASheetObjectWithSourceInfo
	{
		typedef std::vector<ConductionRule> Rules;
		Rules rules;
	};

}
