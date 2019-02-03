#ifndef COMPILER_ERROR_HPP
#define COMPILER_ERROR_HPP

#include <exception>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <fm/exception.hpp>
#include <string>

namespace sheet {
	namespace compiler {
		class Exception : public fm::Exception
		{
		typedef fm::Exception Base;
		public:
			explicit Exception(const std::string& what_) : Base(what_) {} 
			explicit Exception(const std::string& what_, const char *filename, int line) 
				: Base(what_, filename, line)
			{}
			virtual ~Exception() throw () = default;
		};

		namespace handler {

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