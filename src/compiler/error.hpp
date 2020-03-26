#ifndef COMPILER_ERROR_HPP
#define COMPILER_ERROR_HPP

#include <exception>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <fm/exception.hpp>
#include <string>
#include <list>
#include <boost/exception/info.hpp>
#include <sheet/ASheetObject.hpp>
#include <algorithm>
#include <sstream>
#include <fm/common.hpp>

namespace sheet {
	typedef std::list<std::string> Warnings;
	class Document;
	namespace compiler {
		typedef boost::error_info<struct tag_at_object_idx, int> ex_at_object_idx;
		typedef boost::error_info<struct tag_sheet_event, ASheetObjectWithSourceInfo> ex_sheet_source_info;
		typedef boost::error_info<struct tag_sheet_document, std::shared_ptr<Document>> ex_sheet_document;
		typedef boost::error_info<struct tag_error_source_file, fm::String> ex_error_source_file;
		class Exception : public fm::Exception
		{
		typedef fm::Exception Base;
		public:
			explicit Exception(const std::string& what_) : Base(what_) {} 
			explicit Exception(const std::string& what_, const char *filename, int line) 
				: Base(what_, filename, line)
			{}
			virtual ~Exception() throw () = default;
			virtual std::string toString() const override;
			const ASheetObjectWithSourceInfo* getSourceInfo() const;
			const std::string getSourceFile() const;
		protected:
			std::stringstream & strWhere(std::stringstream &ss, const std::string filename, int line = -1) const;
			std::stringstream & strWhat(std::stringstream &ss, const std::string &what) const;
			std::stringstream & strSheetError(std::stringstream &ss, const std::shared_ptr<Document>, const ASheetObjectWithSourceInfo*) const;
		};

		namespace handler {

			void errorHandler(const std::string &source, 
				const std::string &what, 
				int errorPos, 
				ASheetObjectWithSourceInfo::SourceId sourceId);

			template<class Iterator>
			void errorHandler(const boost::fusion::vector<
				Iterator,
				Iterator,
				Iterator,
				boost::spirit::info> &args, 
				ASheetObjectWithSourceInfo::SourceId sourceId)
			{
				using boost::fusion::at_c;
				auto first = at_c<0>(args);
				auto last = at_c<1>(args);
				auto errPos = at_c<2>(args);
				auto what = at_c<3>(args);
				auto source = std::string(first, last);
				errorHandler(source, what.tag, errPos - first, sourceId);
			}

			template<class Iterator>
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
}


#endif