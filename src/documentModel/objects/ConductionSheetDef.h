#pragma once

#include "ASheetObjectWithSourceInfo.h"
#include <com/common.hpp>
#include <vector>
#include <documentModel/PitchDef.h>
#include <variant>
#include <compiler/error.hpp>

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
			typedef std::variant<double, com::String> ValueType;
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
			ValueType value = 0.0;
			double numberValue() const 
			{
				if (value.index() != 0) 
				{
					FM_THROW(compiler::Exception, "invalid value type: number expected");
				}
				return std::get<double>(value); 
			}
			com::String strValue() const
			{ 
				if (value.index() != 1) 
				{
					FM_THROW(compiler::Exception, "invalid value type: quoted string expected");
				}
				return std::get<com::String>(value); 
			}
		};
		typedef std::vector<Declaration> Declarations;
		SelectorsSet selectorsSet;
		Declarations declarations;
	};

	struct ConductionSheetDef : public ASheetObjectWithSourceInfo
	{
		typedef std::vector<ConductionRule> Rules;
		Rules rules;
	};

}
