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
			auto _lineAndPos(const fm::String &file, int sourcePos)
			{
				try {
					auto source = fm::getWerckmeister().openResource(file);
					fm::StreamBuffIterator end;
					fm::StreamBuffIterator begin(*source.get());
					fm::String sourceStr(begin, end);
					auto result = getLineAndPosition(sourceStr, sourcePos, false);
					auto lineNr = getNumberOfLines(sourceStr, sourcePos);
					return std::make_tuple(std::get<0>(result), std::get<1>(result), lineNr);
				} catch(...) {
					return std::make_tuple(fm::String(), -1, -1);
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
			int lineNr = -1;
			std::tie(errorLine, errorPosition, lineNr) = _lineAndPos(sheetfile, ev->sourcePositionBegin);
			fm::String arrowLine(errorPosition, ' ');
			arrowLine += FM_STRING("^~~~~");
			ss << FM_STRING("in file ") << sheetfile << FM_STRING(":") << lineNr + 1 << std::endl
			   << fm::to_wstring(msg_)   << std::endl
			   << errorLine << std:: endl
			   << arrowLine;
			return fm::to_string(ss.str());
		}
	}
}