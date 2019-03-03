#include <boost/test/unit_test.hpp>
#include <iostream>
#include "compiler/parser.h"
#include "compiler/error.hpp"
#include <fm/literals.hpp>
#include <fm/units.hpp>

#include "compiler/lexer.h"
#include <boost/spirit/include/lex_lexertl.hpp>
#include "fm/common.hpp"
#include "testhelper.h"

BOOST_AUTO_TEST_CASE(issue_9_As_und_Es_werden_nicht_erkannt)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
type: sheet;\n\
{\n\
	As Es As7 Es7 Ges Ges7 \n\
}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	
	const auto &chords = defs.tracks[0].voices.begin()->events;

	BOOST_CHECK(checkChord(chords[0], FM_STRING("As")));
	BOOST_CHECK(checkChord(chords[1], FM_STRING("Es")));
	BOOST_CHECK(checkChord(chords[2], FM_STRING("As7")));
	BOOST_CHECK(checkChord(chords[3], FM_STRING("Es7")));
	BOOST_CHECK(checkChord(chords[4], FM_STRING("Ges")));
	BOOST_CHECK(checkChord(chords[5], FM_STRING("Ges7")));


	auto chordelements = chords[0].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::AS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[0].chordDefName() == FM_STRING("X"));

	chordelements = chords[1].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::ES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[1].chordDefName() == FM_STRING("X"));

	chordelements = chords[2].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::AS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[2].chordDefName() == FM_STRING("X7"));

	chordelements = chords[3].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::ES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[3].chordDefName() == FM_STRING("X7"));

	chordelements = chords[4].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::GES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[4].chordDefName() == FM_STRING("X"));

	chordelements = chords[5].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::GES);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[5].chordDefName() == FM_STRING("X7"));

}