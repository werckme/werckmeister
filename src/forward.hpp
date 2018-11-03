#ifndef SHEET_FORWARD_HPP
#define SHEET_FORWARD_HPP

#include <memory>

namespace sheet {
	namespace compiler {
		class Compiler;
		typedef std::shared_ptr<Compiler> CompilerPtr;
	}
	class Document;
	typedef std::shared_ptr<Document> DocumentPtr;
}

#endif
