#include "exception.hpp"
#include <sstream>

namespace com {
	const char* Exception::what() const throw ()
	{
		return msg_.c_str();
	}
	std::string Exception::toString() const 
	{
		std::stringstream ss;
		ss << filename_
			<< "(" << line_ << "): "
			<< msg_;
		return ss.str();
	}
}

