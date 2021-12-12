#pragma once

#include <exception>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <com/exception.hpp>
#include <string>
#include <list>
#include <boost/exception/info.hpp>
#include <documentModel/objects/ASheetObject.h>
#include <algorithm>
#include <sstream>
#include <com/common.hpp>
#include "Warning.hpp"

namespace documentModel
{
	class Document;
}
namespace compiler
{
	typedef boost::error_info<struct tag_sheet_event, documentModel::ASheetObjectWithSourceInfo> ex_sheet_source_info;
	typedef boost::error_info<struct tag_sheet_document, std::shared_ptr<documentModel::Document>> ex_sheet_document;
	typedef boost::error_info<struct tag_error_source_file, com::String> ex_error_source_file;
	class Exception : public com::Exception
	{
		typedef com::Exception Base;

	public:
		explicit Exception(const std::string &what_) : Base(what_) {}
		explicit Exception(const std::string &what_, const char *filename, int line)
			: Base(what_, filename, line)
		{
		}
		virtual ~Exception() throw() = default;
		virtual std::string toString() const override;
		const documentModel::ASheetObjectWithSourceInfo *getSourceInfo() const;
		const std::string getSourceFile() const;

	protected:
		std::ostream &strSheetError(std::ostream &ss, const std::shared_ptr<documentModel::Document>, const documentModel::ASheetObjectWithSourceInfo *) const;
	};

	namespace handler
	{

		void errorHandler(const std::string &source,
						  const std::string &what,
						  int errorPos,
						  documentModel::ASheetObjectWithSourceInfo::SourceId sourceId);

		template <class Iterator>
		void errorHandler(const boost::fusion::vector<
							  Iterator,
							  Iterator,
							  Iterator,
							  boost::spirit::info> &args,
							  documentModel::ASheetObjectWithSourceInfo::SourceId sourceId)
		{
			using boost::fusion::at_c;
			auto first = at_c<0>(args);
			auto last = at_c<1>(args);
			auto errPos = at_c<2>(args);
			auto what = at_c<3>(args);
			auto source = std::string(first, last);
			errorHandler(source, what.tag, errPos - first, sourceId);
		}

		template <class Iterator>
		void errorHandler(const boost::fusion::vector<
						  Iterator,
						  Iterator,
						  Iterator,
						  boost::spirit::info> &args)
		{
			using boost::fusion::at_c;
			// auto first = at_c<0>(args);
			auto last = at_c<1>(args);
			auto errPos = at_c<2>(args);
			auto what = at_c<3>(args);
			auto w = std::string(errPos, last);
			std::stringstream ss;
			ss << "an error occured around line: " << w << " :: " << what.tag;
			throw Exception(ss.str());
		}
	}

}
