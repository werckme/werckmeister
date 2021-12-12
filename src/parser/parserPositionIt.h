#pragma once

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
#include <string>

namespace parser
{

    /**
         * source @see http://marko-editor.com/articles/position_tracking/
         */
    template <typename Iterator>
    struct CurrentPos
    {
        CurrentPos()
        {
            namespace phx = boost::phoenix;
            namespace qi = boost::spirit::qi;
            current_pos = boost::spirit::repository::qi::iter_pos[qi::_val = phx::bind(&CurrentPos::getCurrentPos, this, qi::_1)];
        }

        boost::spirit::qi::rule<Iterator, std::size_t()> current_pos;

        void setStartPos(const Iterator &iterator)
        {
            start_pos_ = iterator;
        }

        std::size_t getCurrentPos(const Iterator &iterator)
        {
            return std::distance(start_pos_, iterator);
        }

    private:
        Iterator start_pos_;
    };

}
