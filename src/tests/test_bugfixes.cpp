#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <com/literals.hpp>
#include <com/units.hpp>

#include "parser/lexer.h"
#include <boost/spirit/include/lex_lexertl.hpp>
#include "com/common.hpp"
#include "testhelper.h"
#include <com/tools.h>

BOOST_AUTO_TEST_CASE(issue_9_As_und_Es_werden_nicht_erkannt)
{
	using namespace com;
	using sheet::PitchDef;
	com::String text = FM_STRING("\
[\n\
type: sheet;\n\
{\n\
	Ab Eb Ab7 Eb7 Gb Gb7 \n\
}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	
	const auto &chords = defs.tracks[0].voices.begin()->events;

	BOOST_CHECK(checkChord(chords[0], FM_STRING("Ab")));
	BOOST_CHECK(checkChord(chords[1], FM_STRING("Eb")));
	BOOST_CHECK(checkChord(chords[2], FM_STRING("Ab7")));
	BOOST_CHECK(checkChord(chords[3], FM_STRING("Eb7")));
	BOOST_CHECK(checkChord(chords[4], FM_STRING("Gb")));
	BOOST_CHECK(checkChord(chords[5], FM_STRING("Gb7")));


	auto chordelements = chords[0].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::AS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[0].chordDefName() == FM_STRING("X"));

	chordelements = chords[1].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::ES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[1].chordDefName() == FM_STRING("X"));

	chordelements = chords[2].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::AS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[2].chordDefName() == FM_STRING("X7"));

	chordelements = chords[3].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::ES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[3].chordDefName() == FM_STRING("X7"));

	chordelements = chords[4].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::GES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[4].chordDefName() == FM_STRING("X"));

	chordelements = chords[5].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::GES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[5].chordDefName() == FM_STRING("X7"));

}

BOOST_AUTO_TEST_CASE(issue_87_asus_chords_will_not_be_recognized)
{
	using namespace com;
	using sheet::PitchDef;
	com::String text = FM_STRING("\
[\n\
type: sheet;\n\
{\n\
	Bsus2 Asus2 \n\
}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 2);
	
	const auto &chords = defs.tracks[0].voices.begin()->events;

	BOOST_CHECK(checkChord(chords[0], FM_STRING("Bsus2")));
	BOOST_CHECK(checkChord(chords[1], FM_STRING("Asus2")));


	auto chordelements = chords[0].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::B);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("sus2"));
	BOOST_CHECK(chords[0].chordDefName() == FM_STRING("Xsus2"));

	chordelements = chords[1].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::A);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("sus2"));
	BOOST_CHECK(chords[1].chordDefName() == FM_STRING("Xsus2"));


}


BOOST_AUTO_TEST_CASE(issue_100_mod_gt_1_in_instrument_config_fails)
{
	/*
	instrumentConf: organ  
    mod staccato
    mod swing 
    volume 63
;
	*/
	std::vector<sheet::Argument> args = {
		makeArg("organ"), 
		makeArg("mod"), makeArg("staccato"), 
		makeArg("mod"), makeArg("swing"),
		makeArg("volume"), makeArg("63")
	};
	std::vector<com::String> keywords = {"mod", "volume"};
	auto keywordsAndValues = com::mapArgumentsByKeywords(args, keywords);
	BOOST_CHECK(keywordsAndValues.size() == 4);
	BOOST_CHECK(keywordsAndValues.count("") == 1);

	auto argsIt = keywordsAndValues.equal_range("").first;
	auto it = argsIt->second.begin();
	BOOST_CHECK((*it).value == "organ");

	argsIt = keywordsAndValues.equal_range("mod").first;
	it = argsIt->second.begin();
	BOOST_CHECK((*it++).value == "staccato");

	++argsIt;
	it = argsIt->second.begin();
	BOOST_CHECK((*it++).value == "swing");

	argsIt = keywordsAndValues.equal_range("volume").first;
	it = argsIt->second.begin();
	BOOST_CHECK((*it++).value == "63");

}