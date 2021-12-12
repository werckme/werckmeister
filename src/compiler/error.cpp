#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <documentModel/objects/Event.h>
#include <documentModel/Document.h>
#include <memory>
#include <com/tools.h>
#include <com/werckmeister.hpp>
#include <com/common.hpp>
#include <iostream>
#include <documentModel/objects/ParserSourceInfo.h>

namespace compiler
{

	std::ostream &Exception::strSheetError(std::ostream &ss,
										   const std::shared_ptr<documentModel::Document> document,
										   const documentModel::ASheetObjectWithSourceInfo *sourceInf) const
	{
		return com::documentMessage(ss, document, sourceInf->sourceId, sourceInf->sourcePositionBegin, msg_);
	}

	const documentModel::ASheetObjectWithSourceInfo *Exception::getSourceInfo() const
	{
		return boost::get_error_info<ex_sheet_source_info>(*this);
	}

	const std::string Exception::getSourceFile() const
	{
		const auto *sourceInf = boost::get_error_info<ex_sheet_source_info>(*this);
		const std::shared_ptr<documentModel::Document> *rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
		if (!rawDocumentPtr)
		{
			return "";
		}
		auto sheetfile = (*rawDocumentPtr)->findSourcePath(sourceInf->sourceId);
		return sheetfile;
	}

	std::string Exception::toString() const
	{
		std::stringstream ss;
		const auto *sourceInf = boost::get_error_info<ex_sheet_source_info>(*this);
		const std::shared_ptr<documentModel::Document> *rawDocumentPtr = boost::get_error_info<ex_sheet_document>(*this);
		const com::String *sourceFile = boost::get_error_info<ex_error_source_file>(*this);
		if (sourceInf && rawDocumentPtr && sourceInf->sourceId != documentModel::Event::UndefinedSource)
		{
			strSheetError(ss, *rawDocumentPtr, sourceInf);
			return ss.str();
		}
		if (sourceFile)
		{
			com::documentMessageWhere(ss, *sourceFile) << std::endl;
			com::documentMessageWhat(ss, msg_) << std::endl;
			return ss.str();
		}
		return Base::toString();
	}

	namespace handler
	{

		void errorHandler(const std::string &source,
						  const std::string &what,
						  int errorPos,
						  documentModel::ASheetObjectWithSourceInfo::SourceId sourceId)
		{
			std::string line;
			int linePos = 0;
			std::tie(line, linePos) = com::getLineAndPosition<std::string>(source, errorPos, false);
			auto sourceInfo = documentModel::ParserSourceInfo();
			sourceInfo.sourceId = sourceId;
			sourceInfo.sourcePositionBegin = errorPos;
			throw Exception("syntax error") << ex_sheet_source_info(sourceInfo);
		}

	}
}