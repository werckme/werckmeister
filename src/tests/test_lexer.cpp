#include <boost/test/unit_test.hpp>
#include "parser/lexer.h"
#include <boost/spirit/include/lex_lexertl.hpp>
#include <iostream>
#include "fm/common.hpp"

BOOST_AUTO_TEST_CASE(test_chorddef)
{
	using namespace sheet::compiler;
	ChordDefTokenizer<LexerType> chordDefTok;

	fm::String str(FM_STRING("--here goes comment 1\n\
  \t@import 'old.chdef';\r\n\
@import 'old2.chdef'; --here goes comment 2\n\
@import 'old3.chdef'; \n\
	-- 4th\n\
Xmaj: I=1 III=5 V=8\n\
X7: I=1 III=5 V=8 VII=10 --as usual\n\
\n\
\r\n\
   \n\
  \t\r\n\
	Xmaj7 : I=1 III=5 V=8 VII=11\n\
	X7 + : Xmaj7\n\
"));
	
	fm::CharType const* first = str.c_str();
	fm::CharType const* last = &first[str.size()];

	LexerType::iterator_type iter = chordDefTok.begin(first, last);
	LexerType::iterator_type end = chordDefTok.end();

	boost::spirit::lex::tokenize(first, last, chordDefTok);
	BOOST_CHECK( chordDefTok.documentConfigs.size() == 3 );
	//for (const auto &x : chordDefTok.documentConfigs) {
	//	FM_COUT << x << std::endl;
	//}
	BOOST_CHECK(chordDefTok.chordDefs.size() == 4);
	//for (const auto &x : chordDefTok.chordDefs) {
	//	FM_COUT << x << std::endl;
	//}
}

BOOST_AUTO_TEST_CASE(test_pitchmap)
{
	using namespace sheet::compiler;
	PitchmapTokenizer<LexerType> pitchmapTok;

	fm::String str(FM_STRING("\
\"bd\": c,,\n\
\"sn\": e,\n\
"));

	fm::CharType const* first = str.c_str();
	fm::CharType const* last = &first[str.size()];

	LexerType::iterator_type iter = pitchmapTok.begin(first, last);
	LexerType::iterator_type end = pitchmapTok.end();

	boost::spirit::lex::tokenize(first, last, pitchmapTok);
	BOOST_CHECK(pitchmapTok.documentConfigs.size() == 0);
	//for (const auto &x : chordDefTok.documentConfigs) {
	//	FM_COUT << x << std::endl;
	//}
	BOOST_CHECK(pitchmapTok.pitchdefs.size() == 2);
	//for (const auto &x : chordDefTok.chordDefs) {
	//	FM_COUT << x << std::endl;
	//}
}
