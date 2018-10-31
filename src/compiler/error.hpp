#ifndef COMPILER_ERROR_HPP
#define COMPILER_ERROR_HPP

#include <exception>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/info.hpp>

namespace sheet {
	namespace compiler {
		class Exception : public std::exception
		{
		public:
			explicit Exception(const std::string& what_) : msg_(what_)
			{}
			virtual ~Exception() throw () {}
			virtual const char* what() const throw () {
				return msg_.c_str();
			}

		protected:
			std::string msg_;
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