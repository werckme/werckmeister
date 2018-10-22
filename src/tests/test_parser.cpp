#include <boost/test/unit_test.hpp>
#include <iostream>
#include "compiler/parser.h"


BOOST_AUTO_TEST_CASE(test_chordDefparser)
{
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
x7+ : 1 5 8 11\n\
X/V: -6 1 5 8 --quinte im bass\
"));
	
	sheet::compiler::ChordDefParser defParser;
	
	auto chordDefs = defParser.parse(str);
	BOOST_TEST( chordDefs.size() == 5 );
	auto &def = chordDefs[0];
	BOOST_TEST( (chordDefs[0].name == FM_STRING("Xmaj")));
	auto interval = def.intervals.begin();
	BOOST_TEST((interval++)->value == 1);
	BOOST_TEST((interval++)->value == 5);
	BOOST_TEST((interval++)->value == 8);
	BOOST_TEST((interval == chordDefs[0].intervals.end()));


	BOOST_TEST((chordDefs[1].name == FM_STRING("X7")));
	interval = chordDefs[1].intervals.begin();
	BOOST_TEST((interval++)->value == 1);
	BOOST_TEST((interval++)->value == 5);
	BOOST_TEST((interval++)->value == 8);
	BOOST_TEST((interval++)->value == 10);
	BOOST_TEST((interval == chordDefs[1].intervals.end()));


	BOOST_TEST((chordDefs[2].name == FM_STRING("Xmaj7")));
	interval = chordDefs[2].intervals.begin();
	BOOST_TEST((interval++)->value == 1);
	BOOST_TEST((interval++)->value == 5);
	BOOST_TEST((interval++)->value == 8);
	BOOST_TEST((interval++)->value == 11);
	BOOST_TEST((interval == chordDefs[2].intervals.end()));

	BOOST_TEST((chordDefs[3].name == FM_STRING("x7+")));
	interval = chordDefs[3].intervals.begin();
	BOOST_TEST((interval++)->value == 1);
	BOOST_TEST((interval++)->value == 5);
	BOOST_TEST((interval++)->value == 8);
	BOOST_TEST((interval++)->value == 11);
	BOOST_TEST((interval == chordDefs[3].intervals.end()));

	BOOST_TEST((chordDefs[4].name == FM_STRING("X/V")));
	interval = chordDefs[4].intervals.begin();
	BOOST_TEST((interval++)->value == -6);
	BOOST_TEST((interval++)->value == 1);
	BOOST_TEST((interval++)->value == 5);
	BOOST_TEST((interval++)->value == 8);
	BOOST_TEST((interval == chordDefs[4].intervals.end()));
}


BOOST_AUTO_TEST_CASE(test_styleDefparser)
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
\n\
	section normal\n\
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
	sheet::compiler::StyleDefParser parser;
}