#include <boost/test/unit_test.hpp>
#include "compiler/lexer.h"
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
Xmaj: 1 5 8\n\
X7: 1 5 8 10 --as usual\n\
\n\
\r\n\
   \n\
  \t\r\n\
	Xmaj7 : 1 5 8 11\n\
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

BOOST_AUTO_TEST_CASE(test_styledef)
{
	fm::String text = FM_STRING("\
		@use anything; \n\
		@or not; \n\
	--some useless comment\n\
		section intro -- section \n\
		[--track 1 begin\n\
		/ name:piano / --meta informationen\n\
		/ soundselect : 0 0 /\n\
		/ channel : 1 /\n\
	{ I4 II4 III4 IV4 | } --a separate voice / similar to lilypond\n\
	{ I,4 II,4 III,4 IV,4 }\n\
		] --track 1 end\n\
		[\n\
			/ name:bass /\n\
			/ soundselect : 0 0 /\n\
		/ channel : 2 /\n\
		I4 I'4 III4 I'4\n\
		]\n\
		end\n\
-\n\
--\n\
	section normal--\n\
		  [--track 1 begin\n\
			  / name:piano / --meta informationen\n\
			  / soundselect : 0 0 / \n\
			  / channel : 1 / \n\
			  / signature : 4 | 4 / \n\
		  { I4 II4 III4 IV4 | } --a separate voice / similar to lilypond\n\
		  { I8 <III V VII>2.. | }\n\
		  ] --track 1 end\n\
			  [\n\
				  / name:bass / \n\
				  I4 I'4 III4 I'4\n\
			  ]\n\
end");
	using namespace sheet::compiler;
	StyleDefTokenizer<LexerType> styleDefToken;

	fm::CharType const* first = text.c_str();
	fm::CharType const* last = &first[text.size()];

	LexerType::iterator_type iter = styleDefToken.begin(first, last);
	LexerType::iterator_type end = styleDefToken.end();
	boost::spirit::lex::tokenize(first, last, styleDefToken);
	BOOST_CHECK(styleDefToken.documentConfigs.size() == 2 );
	for (const auto &x : styleDefToken.documentConfigs) {
		//FM_COUT << x << std::endl;
	}
	BOOST_CHECK(styleDefToken.sections.size() == 2);
	for (const auto &x : styleDefToken.sections) {
		//FM_COUT << "\n\n!\n\n" << x << std::endl;
	}

}