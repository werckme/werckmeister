#ifndef SHEET_DOCUMENTPARSER_HPP
#define SHEET_DOCUMENTPARSER_HPP

#include "forward.hpp"
#include <fm/common.hpp>
#include <memory>

namespace sheet {
	namespace compiler {        
        
        class IDocumentParser {
		public:
			virtual DocumentPtr parse(const fm::String &path) = 0;
		};
		typedef std::shared_ptr<IDocumentParser> IDocumentParserPtr;
    }
}

#endif