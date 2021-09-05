#ifndef SHEET_STYLE_SHEET_H
#define SHEET_STYLE_SHEET_H

#include <fm/common.hpp>
#include <list>

namespace sheet {

	struct ConductionSelector {
		enum Type { Unkown };
		Type type = Unkown;
	};

	struct ConductionRule {
		ConductionSelector selector;
	};

	struct ConductionSheetDef {
		enum {
			UndefinedSource = 0,
			UndefinedPosition = INT_MAX
		};
		typedef unsigned int SourceId;
		SourceId sourceId = UndefinedSource;
		typedef std::list<ConductionRule> Rules;
		Rules rules;
	};

}

#endif