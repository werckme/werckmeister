#include <boost/test/unit_test.hpp>
#include <fm/common.hpp>
#include <fm/midi.hpp>
#include "compiler/lexer.h"
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(test_lexer01)
{

	fm::word_count_tokens<fm::lexer_type> word_count_lexer;

	std::string str("@rule1 abba\n@rule2\n#comment\n@rule3");
	char const* first = str.c_str();
	char const* last = &first[str.size()];

	fm::lexer_type::iterator_type iter = word_count_lexer.begin(first, last);
	fm::lexer_type::iterator_type end = word_count_lexer.end();

	boost::spirit::lex::tokenize(first, last, word_count_lexer);

	for (const auto &x : word_count_lexer.found) {
		std::cout << x << std::endl;
	}
}