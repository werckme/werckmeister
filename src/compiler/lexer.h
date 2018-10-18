#ifndef FM_LEXER_HPP
#define FM_LEXER_HPP

#include <algorithm>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/bind.hpp>
#include <vector>

namespace fm {

	typedef
		boost::spirit::lex::lexertl::token<char const*, boost::spirit::lex::omit, boost::mpl::false_> token_type;

	typedef boost::spirit::lex::lexertl::actor_lexer<token_type> lexer_type;

	template <typename Lexer>
	struct word_count_tokens : boost::spirit::lex::lexer<Lexer>
	{

		void add(char const *begin, char const *end) {
			found.push_back(std::string(begin, end));
		}

		word_count_tokens()
			: word("@.+$")     // define tokens
			, comment("#.+$")
			, eol("\n")
			, any(".")
		{
			using boost::spirit::lex::_start;
			using boost::spirit::lex::_end;
			using boost::phoenix::ref;
			auto f = boost::bind(&word_count_tokens::add, this, _1, _2);
			// associate tokens with the lexer
			this->self
				= (word[f] | comment)
				| eol
				| any
				;
		}
		std::vector<std::string> found;
		boost::spirit::lex::token_def<> word, eol, any, comment;
	};
}

#endif