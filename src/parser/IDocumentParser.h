#pragma once

#include "forward.hpp"
#include <com/common.hpp>
#include <memory>

namespace parser
{

	class IDocumentParser
	{
	public:
		virtual documentModel::DocumentPtr parse(const com::String &path) = 0;
	};
	typedef std::shared_ptr<IDocumentParser> IDocumentParserPtr;
}
