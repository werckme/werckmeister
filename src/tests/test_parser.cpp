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
X7+ : Xmaj7\n\
X/V: -6 1 5 8 --quinte im bass\
"));
	
	sheet::compiler::ChordDefParser defParser;
	auto chordDefs = defParser.parse(str);
	BOOST_TEST( chordDefs.size() == 5 );
	BOOST_TEST( (chordDefs[0].name == FM_STRING("Xmaj")));
	BOOST_TEST((chordDefs[1].name == FM_STRING("X7")));
	BOOST_TEST((chordDefs[2].name == FM_STRING("Xmaj7")));
	BOOST_TEST((chordDefs[3].name == FM_STRING("X7+")));
	BOOST_TEST((chordDefs[4].name == FM_STRING("X/V")));
}