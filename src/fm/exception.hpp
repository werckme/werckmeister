#ifndef FM_EXCEPTION_HPP
#define FM_EXCEPTION_HPP

#include <exception>
#include <string>
#include <sstream>

namespace fm {
    class Exception : public std::exception
    {
    public:
        explicit Exception(const std::string &what_) : msg_(what_) {}
        explicit Exception(const std::string& what_, const char *filename, int line)
        {
            std::stringstream ss;
            ss << filename
               << "(" << line << "): "
               << what_;
            msg_ = ss.str();
        }
        virtual ~Exception() throw () {}
        virtual const char* what() const throw () {
            return msg_.c_str();
        }

    protected:
        std::string msg_;
    };
}

#endif