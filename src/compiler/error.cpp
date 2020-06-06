#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <sheet/objects/Event.h>
#include <sheet/Document.h>
#include <memory>
#include <fm/tools.h>
#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include <iostream>
#include <sheet/objects/ParserSourceInfo.h>

namespace sheet {
	namespace compiler {

		std::stringstream & Exception::strSheetError(std::stringstream &ss, 
			const std::shared_ptr<Document> document, 
			const ASheetObjectWithSourceInfo* sourceInf) const
		{
			return fm::documentMessage(ss, document, sourceInf->sourceId, sourceInf->sourcePositionBegin, msg_);
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
				fm::documentMessageWhere(ss, *sourceFile) << std::endl;
				fm::documentMessageWhat(ss, msg_) << std::endl;
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
				std::tie(line, linePos) = fm::getLineAndPosition<std::string>(source, errorPos, false);
				auto sourceInfo = ParserSourceInfo();
				sourceInfo.sourceId = sourceId;
				sourceInfo.sourcePositionBegin = errorPos;
				throw Exception( "syntax error" ) <<  ex_sheet_source_info(sourceInfo);
			}

		}
	}
}