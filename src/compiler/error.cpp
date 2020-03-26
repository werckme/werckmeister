#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <sheet/Event.h>
#include <sheet/Document.h>
#include <memory>
#include <sheet/tools.h>
#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include <iostream>

namespace sheet {
	namespace compiler {

		namespace {
			auto _lineAndPos(const fm::String &file, unsigned int sourcePos)
			{
				if (sourcePos == ASheetObjectWithSourceInfo::UndefinedPosition) {
					sourcePos = 0;
				}
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

		std::stringstream & Exception::strWhere(std::stringstream &ss, const std::string filename, int line) const
		{
			ss << "in file " << filename;
			if (line > 0) {
				ss << ":" << line;
			} 
			return ss;
		}

		std::stringstream & Exception::strWhat(std::stringstream &ss, const std::string &what) const
		{
			ss << what;
			return ss;
		}

		std::stringstream & Exception::strSheetError(std::stringstream &ss, 
			const std::shared_ptr<Document> document, 
			const ASheetObjectWithSourceInfo* sourceInf) const
		{
			auto sheetfile = document->findSourcePath(sourceInf->sourceId);
			fm::String errorLine;
			int errorPosition = -1;
			int lineNr = -1;
			std::tie(errorLine, errorPosition, lineNr) = _lineAndPos(sheetfile, sourceInf->sourcePositionBegin);
			std::string arrowLine(errorPosition, ' ');
			arrowLine += "^~~~~";
			strWhere(ss, sheetfile, lineNr+1) << std::endl;
			strWhat(ss, msg_) << std::endl
			   << errorLine << std::endl
			   << arrowLine;
			return ss;
		}

		const ASheetObjectWithSourceInfo * Exception::getSourceInfo() const
		{
			return boost::get_error_info<ex_sheet_source_info>(*this);
		}

		const std::string Exception::getSourceFile() const
		{
			const auto *sourceInf 							 = boost::get_error_info<ex_sheet_source_info>(*this);
			const std::shared_ptr<Document> * rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
			if (!rawDocumentPtr) {
				return "";
			}
			auto sheetfile = (*rawDocumentPtr)->findSourcePath(sourceInf->sourceId);
			return sheetfile;
		}

		std::string Exception::toString() const
		{
			std::stringstream ss;
			const auto *sourceInf 							 = boost::get_error_info<ex_sheet_source_info>(*this);
			const std::shared_ptr<Document> * rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
			const fm::String *sourceFile 					 = boost::get_error_info<ex_error_source_file>(*this);
			if (sourceInf && rawDocumentPtr && sourceInf->sourceId != Event::UndefinedSource) {
				strSheetError(ss, *rawDocumentPtr, sourceInf);
				return ss.str();
			}
			if (sourceFile) {
				strWhere(ss, *sourceFile) << std::endl;
				strWhat(ss, msg_) << std::endl;
				return ss.str();
			}
			return Base::toString();
		}
		
		namespace handler {

			void errorHandler(const std::string &source, 
				const std::string &what, 
				int errorPos, 
				ASheetObjectWithSourceInfo::SourceId sourceId)
			{
				std::string line;
				int linePos = 0;
				std::tie(line, linePos) = getLineAndPosition<std::string>(source, errorPos, false);
				auto sourceInfo = ParserSourceInfo();
				sourceInfo.sourceId = sourceId;
				sourceInfo.sourcePositionBegin = errorPos;
				throw Exception( "syntax error" ) <<  ex_sheet_source_info(sourceInfo);
			}

		}
	}
}