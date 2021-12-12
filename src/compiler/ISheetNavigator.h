#pragma once

#include "forward.hpp"
#include <memory>

namespace documentModel
{
	struct Voice;
}
namespace compiler
{
	/// Is responsible for solving navigation commands such as mark/jump etc.
	class ISheetNavigator
	{
	public:
		virtual void processNavigation(documentModel::Voice &) = 0;
	};
	typedef std::shared_ptr<compiler::ISheetNavigator> ISheetNavigatorPtr;
}
