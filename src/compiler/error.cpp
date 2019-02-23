#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <sheet/Event.h>
#include <sheet/Document.h>
#include <memory>
#include <sheet/tools.h>
#include <fm/werckmeister.hpp>
#include <fm/common.hpp>

namespace sheet {
	namespace compiler {

		namespace {
			LineAndPosition<fm::String> _lineAndPos(const fm::String &file, int sourcePos)
			{
				try {
					auto source = fm::getWerckmeister().openResource(file);
					fm::StreamBuffIterator end;
					fm::StreamBuffIterator begin(*source.get());
					fm::String sourceStr(begin, end);
					return getLineAndPosition(sourceStr, sourcePos, false);
				} catch(...) {
					return LineAndPosition<fm::String>(fm::String(), -1);
				}
			}
		}

		std::string Exception::toString() const
		{
			const Event *ev = boost::get_error_info<ex_sheet_event>(*this);
			const std::shared_ptr<Document> * rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
			if (!ev || !rawDocumentPtr || ev->sourceId == Event::UndefinedSource) {
				return Base::toString();
			}
			auto document = *rawDocumentPtr;
			fm::StringStream ss;
			auto sheetfile = document->findSourcePath(ev->sourceId);
			fm::String errorLine;
			int errorPosition = -1;
			std::tie(errorLine, errorPosition) = _lineAndPos(sheetfile, ev->sourcePositionBegin);
			fm::String arrowLine(errorPosition, ' ');
			arrowLine += FM_STRING("^~~~~");
			ss << FM_STRING("in file ") << sheetfile << FM_STRING(":") << std::endl
			   << fm::to_wstring(msg_)   << std::endl
			   << errorLine << std:: endl
			   << arrowLine;
			return fm::to_string(ss.str());
		}
	}
}