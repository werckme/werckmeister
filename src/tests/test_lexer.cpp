#include <boost/test/unit_test.hpp>
#include "compiler/lexer.h"
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(test_chorddef)
{
	using namespace sheet::compiler;
	ChordDefTokenizer<lexer_type> chordDefTok;

	fm::String str(FM_STRING("--here goes comment 1\n\
  \t@import 'old.chdef';\r\n\
@import 'old2.chdef'; --here goes comment 2\n\
@import 'old3.chdef'; \n\
Xmaj: 1 5 8\n\
X7: 1 5 8 10\n\
\n\
\r\n\
   \n\
  \t\r\n\
Xmaj7 : 1 5 8 11\n\
X7 + : Xmaj7\n\
"));
	
	fm::CharType const* first = str.c_str();
	fm::CharType const* last = &first[str.size()];

	lexer_type::iterator_type iter = chordDefTok.begin(first, last);
	lexer_type::iterator_type end = chordDefTok.end();

	boost::spirit::lex::tokenize(first, last, chordDefTok);
	BOOST_TEST( chordDefTok.documentConfigs.size() == 3 );
	for (const auto &x : chordDefTok.documentConfigs) {
		std::wcout << x << std::endl;
	}
	BOOST_TEST(chordDefTok.comments.size() == 2);
	for (const auto &x : chordDefTok.comments) {
		std::wcout << x << std::endl;
	}
	BOOST_TEST(chordDefTok.chordDefs.size() == 4);
	for (const auto &x : chordDefTok.chordDefs) {
		std::wcout << x << std::endl;
	}
}