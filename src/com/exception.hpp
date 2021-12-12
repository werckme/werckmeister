#pragma once

#include <exception>
#include <string>
#include <boost/exception/exception.hpp>

namespace com
{
	class Exception : public virtual std::exception, public boost::exception
	{
	public:
		explicit Exception(const std::string &what_) : msg_(what_) {}
		explicit Exception(const std::string &what_, const char *filename, int line)
			: msg_(what_), filename_(filename), line_(line)
		{
		}
		virtual ~Exception() throw() {}
		virtual const char *what() const throw();
		virtual std::string toString() const;

	protected:
		std::string msg_, filename_;
		int line_ = -1;
	};
}
