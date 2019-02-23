#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <sheet/Event.h>
#include <sheet/Document.h>
#include <memory>
#include <sstream>

namespace sheet {
	namespace compiler {
		std::string Exception::toString() const
		{
			const Event *ev = boost::get_error_info<ex_sheet_event>(*this);
			const std::shared_ptr<Document> * rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
			if (!ev || !rawDocumentPtr || ev->sourceId == Event::UndefinedSource) {
				return Base::toString();
			}
			auto document = *rawDocumentPtr;
			std::stringstream ss;
			auto sheetfile = document->findSourcePath(ev->sourceId);
			ss << fm::to_string(sheetfile) << ":"
			   << msg_;
			return ss.str();
		}
	}
}