#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "testhelper.h"

BOOST_AUTO_TEST_CASE(test_argQuoted)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: \"value\";		\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
}

BOOST_AUTO_TEST_CASE(test_argNameQuotedValue)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: _name=\"value\";	 	\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
	BOOST_CHECK(meta.args[0].name == "name");
}

BOOST_AUTO_TEST_CASE(test_argNameValue)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: _name=value;		\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
	BOOST_CHECK(meta.args[0].name == "name");
}

BOOST_AUTO_TEST_CASE(test_argUnqouted)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: value;		\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
}

BOOST_AUTO_TEST_CASE(test_argNameValue2)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: _name=value _name2=value2;		\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK_EQUAL(meta.args.size(), (size_t)2); 
	BOOST_CHECK(meta.args[0].value == "value");
	BOOST_CHECK(meta.args[0].name == "name");
	BOOST_CHECK(meta.args[1].value == "value2");
	BOOST_CHECK(meta.args[1].name == "name2");
}

BOOST_AUTO_TEST_CASE(test_argNameValue3)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
key: myArpeggio direction down _style=legato2;		\
[{			    \
}]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK_EQUAL(meta.args.size(), (size_t)4); 
	BOOST_CHECK(meta.args[0].value == "myArpeggio");
	BOOST_CHECK(meta.args[0].name == "");

	BOOST_CHECK(meta.args[1].value == "direction");
	BOOST_CHECK(meta.args[1].name == "");

	BOOST_CHECK(meta.args[2].value == "down");
	BOOST_CHECK(meta.args[2].name == "");

	BOOST_CHECK(meta.args[3].value == "legato2");
	BOOST_CHECK(meta.args[3].name == "style");
}

BOOST_AUTO_TEST_CASE(test_trackArgQuoted)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[				\
key: \"value\";		\
{			    \
}				\
]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value") );
}

BOOST_AUTO_TEST_CASE(test_trackArgQuoted2)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[				\
key: \"value two\";		\
{			    \
}				\
]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value two"));
}

BOOST_AUTO_TEST_CASE(test_trackArgUnquoted)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[				\
key: value;		\
{			    \
}				\
]			    \
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value") );
}

BOOST_AUTO_TEST_CASE(test_chordDefparser)
{
	fm::String str(FM_STRING("--here goes comment 1\n\
  \t@import 'old.chdef';\r\n\
@import 'old2.chdef'; --here goes comment 2\n\
@import 'old3.chdef'; \n\
Xmaj: I=1 III=5 V=8\n\
X7: I=1 III=5 V=8 VII=10\n\
\n\
\r\n\
   \n\
  \t\r\n\
Xmaj7 : I=1 III=5 V=8 VII=11\n\
X7+ : I=1 III=5 V=8 VII=11\n\
X/V: I=1 III=5 V=-6  --quinte im bass\
"));

	sheet::compiler::ChordDefParser defParser;
	using namespace fm::degrees;

	auto chordDefs = defParser.parse(str);
	BOOST_CHECK(chordDefs.size() == 5);
	auto &def = chordDefs[0];
	BOOST_CHECK((chordDefs[0].name == FM_STRING("Xmaj")));
	auto interval = def.intervals.begin();
	BOOST_CHECK((interval)->degree == I);
	BOOST_CHECK((interval++)->value == 1);

	BOOST_CHECK((interval)->degree == III);
	BOOST_CHECK((interval++)->value == 5);

	BOOST_CHECK((interval)->degree == V);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval == chordDefs[0].intervals.end()));


	BOOST_CHECK((chordDefs[1].name == FM_STRING("X7")));
	interval = chordDefs[1].intervals.begin();

	BOOST_CHECK((interval)->degree == I);
	BOOST_CHECK((interval++)->value == 1);

	BOOST_CHECK((interval)->degree == III);
	BOOST_CHECK((interval++)->value == 5);

	BOOST_CHECK((interval)->degree == V);
	BOOST_CHECK((interval++)->value == 8);

	BOOST_CHECK((interval)->degree == VII);
	BOOST_CHECK((interval++)->value == 10);
	BOOST_CHECK((interval == chordDefs[1].intervals.end()));


	BOOST_CHECK((chordDefs[2].name == FM_STRING("Xmaj7")));
	interval = chordDefs[2].intervals.begin();

	BOOST_CHECK((interval)->degree == I);
	BOOST_CHECK((interval++)->value == 1);

	BOOST_CHECK((interval)->degree == III);
	BOOST_CHECK((interval++)->value == 5);

	BOOST_CHECK((interval)->degree == V);
	BOOST_CHECK((interval++)->value == 8);

	BOOST_CHECK((interval)->degree == VII);
	BOOST_CHECK((interval++)->value == 11);
	BOOST_CHECK((interval == chordDefs[2].intervals.end()));

	BOOST_CHECK((chordDefs[3].name == FM_STRING("X7+")));
	interval = chordDefs[3].intervals.begin();

	BOOST_CHECK((interval)->degree == I);
	BOOST_CHECK((interval++)->value == 1);

	BOOST_CHECK((interval)->degree == III);
	BOOST_CHECK((interval++)->value == 5);

	BOOST_CHECK((interval)->degree == V);
	BOOST_CHECK((interval++)->value == 8);

	BOOST_CHECK((interval)->degree == VII);
	BOOST_CHECK((interval++)->value == 11);


	BOOST_CHECK((interval == chordDefs[3].intervals.end()));

	BOOST_CHECK((chordDefs[4].name == FM_STRING("X/V")));
	interval = chordDefs[4].intervals.begin();

	BOOST_CHECK((interval)->degree == I);
	BOOST_CHECK((interval++)->value == 1);

	BOOST_CHECK((interval)->degree == III);
	BOOST_CHECK((interval++)->value == 5);

	BOOST_CHECK((interval)->degree == V);
	BOOST_CHECK((interval++)->value == -6);


	BOOST_CHECK((interval == chordDefs[4].intervals.end()));
}

BOOST_AUTO_TEST_CASE(test_SheetDefParser)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	--some useless comment\n\
[--a track\n\
	{\n\
		I,4 II,,8 III,,,16 IV32 | I,4 I,, I,,, I | r1 | <I' III' V'>4 \n\
		/name: bass/\n\
		/soundselect: \"0\" \"0\"/\n\
		/acommand: _first=arg1 _second=\"arg2\"/\n\
	} -- a voice\n\
	{\n\
		IV'4. VII''8. I'''16. II32. | II'4 II'' II''' II | r1 \n\
	}-- further voice \n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 16);
	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 11);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Degree, 4, 0, 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::Degree, 1, -2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Degree, 1, -3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Degree, 1, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[12], sheet::Event::Degree, sheet::Event::Pitches({ PitchDef(1, 1), PitchDef(3, 1), PitchDef(5, 1) }), 1.0_N4));

	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[13].metaArgs.size(), (size_t)1);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[13], FM_STRING("name"), sheet::Event::Args({ makeArg("bass") })));

	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[14].metaArgs.size(), (size_t)2);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[14], FM_STRING("soundselect"), sheet::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[15].metaArgs.size(), (size_t)2);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[15], FM_STRING("acommand"), sheet::Event::Args({ makeArg("first", "arg1"), makeArg("second", "arg2") })));

	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], sheet::Event::Degree, 4, 1, 1.0_N4p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], sheet::Event::Degree, 7, 2, 1.0_N8p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], sheet::Event::Degree, 1, 3, 1.0_N16p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], sheet::Event::Degree, 2, 0, 1.0_N32p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[5], sheet::Event::Degree, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[6], sheet::Event::Degree, 2, 2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[7], sheet::Event::Degree, 2, 3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[8], sheet::Event::Degree, 2, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));
}


BOOST_AUTO_TEST_CASE(test_SheetDefParser_mixed_with_absolute_notes)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	--some useless comment\n\
[--a track\n\
	{\n\
		I,4 II,,8 III,,,16 IV32 \n\
	} -- a voice\n\
	{\n\
		c4 d4 e4 f4 \n\
	}-- further voice \n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Degree, 4, 0, 1.0_N32));

	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 4);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], sheet::Event::Note, fm::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], sheet::Event::Note, fm::notes::D, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], sheet::Event::Note, fm::notes::E, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], sheet::Event::Note, fm::notes::F, 0, 1.0_N4));
}


BOOST_AUTO_TEST_CASE(test_alias_notes)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				\"bd\"4 \"sn\"8	<\"ht\" \"cymbal1\" >8\n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, FM_STRING("bd"), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Note, FM_STRING("sn"), 1.0_N8));
	BOOST_CHECK(defs.tracks[0].voices[0].events[2].pitches.size() == 2);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(FM_STRING("ht")), PitchDef(FM_STRING("cymbal1")) }), 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_alias_sheetdef)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				\"bd\"4 \"sn\"8	<\"ht\" \"cymbal1\" >8\n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, FM_STRING("bd"), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Note, FM_STRING("sn"), 1.0_N8));
	BOOST_CHECK(defs.tracks[0].voices[0].events[2].pitches.size() == 2);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(FM_STRING("ht")), PitchDef(FM_STRING("cymbal1")) }), 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_SheetDefParser_fail)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	--some useless comment\n\
[--a track\n\
	{\n\
		I,4 II,,8 III,,,16 IV32 | I,4 I,, I,,, I | r1 | <I' III' V'>4 \n\
		/name: bass/\n\
		/soundselect: 0 0/\n\
		/acommand: arg1 arg2/\n\
	} -- a voice\n\
	{\n\
		IV'4. VII''8. I'''16. II32. | II'4 II'' II''' IIt --here is the error (t is not valid without a value)\n\
 | r1 \n\
	}-- further voice \n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);
}


BOOST_AUTO_TEST_CASE(test_sheetDefParser)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
	{\n\
		<c e g>4 f4 f4 f4 | b4 b4 b4 b4 | \n\
	}\n\
]\n\
-- the sheet, no tracks and voices here\n\
[\n\
type: sheet;\n\
{\n\
	/ sheetTemplate : simplePianoSheetTemplate Intro / \n\
	/ voicingStrategy : asNotated / \n\
	Cmaj | Cmaj C7 | r G | A# B | C#maj79 r Ai \n\
}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 10);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({  makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[5], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[6], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[7], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[8], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[9], sheet::Event::EOB));

	const auto &chords = defs.tracks[1].voices[0].events;

	BOOST_CHECK(chords.size() == 16);
	BOOST_CHECK(checkMetaEvent(chords[0], FM_STRING("sheetTemplate"), sheet::Event::Args({ makeArg("simplePianoSheetTemplate"), makeArg("Intro") })));
	BOOST_CHECK(checkMetaEvent(chords[1], FM_STRING("voicingStrategy"), sheet::Event::Args({ makeArg("asNotated") })));
	BOOST_CHECK(checkChord(chords[2], FM_STRING("Cmaj")));
	BOOST_CHECK(checkNote(chords[3], sheet::Event::EOB));
	BOOST_CHECK(checkChord(chords[4], FM_STRING("Cmaj")));
	BOOST_CHECK(checkChord(chords[5], FM_STRING("C7")));
	BOOST_CHECK(checkNote(chords[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(chords[7], sheet::Event::Rest));
	BOOST_CHECK(checkChord(chords[8], FM_STRING("G")));
	BOOST_CHECK(checkNote(chords[9], sheet::Event::EOB));
	BOOST_CHECK(checkChord(chords[10], FM_STRING("A#")));
	BOOST_CHECK(checkChord(chords[11], FM_STRING("B")));
	BOOST_CHECK(checkNote(chords[12], sheet::Event::EOB));
	BOOST_CHECK(checkChord(chords[13], FM_STRING("C#maj79")));
	BOOST_CHECK(checkNote(chords[14], sheet::Event::Rest));
	BOOST_CHECK(checkChord(chords[15], FM_STRING("Ai")));

	auto chordelements = chords[2].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::C);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("maj"));
	BOOST_CHECK(chords[2].chordDefName() == FM_STRING("Xmaj"));

	chordelements = chords[5].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::C);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[5].chordDefName() == FM_STRING("X7"));

	chordelements = chords[8].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::G);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[8].chordDefName() == FM_STRING("X"));

	chordelements = chords[10].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::AIS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[10].chordDefName() == FM_STRING("X"));

	chordelements = chords[13].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::CIS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("maj79"));
	BOOST_CHECK(chords[13].chordDefName() == FM_STRING("Xmaj79"));

	chordelements = chords[15].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == fm::notes::A);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("i"));
	BOOST_CHECK(chords[15].chordDefName() == FM_STRING("Xi"));

}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_02)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_03)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
	  -- Antônio Carlos Jobim\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
[\n\
	{\n\
		<c e g>4 f4 f4 f4 | b4 b4 b4 b4 | \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

	BOOST_CHECK(defs.tracks[1].voices.size() == 1);
	BOOST_CHECK(defs.tracks[1].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[0], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[1], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[2], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[3], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[5], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[6], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[7], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[8], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[9], sheet::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_expression_meta)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		c4\\pp d4 e4 f4 | \\ffff c'4 d'4 e'4 !p f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 13);

	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, fm::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("expression"), sheet::Event::Args({ makeArg("pp") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, fm::notes::D, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, fm::notes::E, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, fm::notes::F, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::EOB));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[6], FM_STRING("expression"), sheet::Event::Args({ makeArg("ffff") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, fm::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, fm::notes::D, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, fm::notes::E, 1, 1.0_N4));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[10], FM_STRING("expressionPlayedOnce"), sheet::Event::Args({ makeArg("p") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::Note, fm::notes::F, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[12], sheet::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_vorschlag)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
	 d'32 -> c'4 r2.|\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("addVorschlag"), sheet::Event::Args({})));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Meta, fm::notes::D, 1, 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Note, fm::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N2p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::EOB));
}

BOOST_AUTO_TEST_CASE(test_alias_vorschlag)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
	 \"bd\"32->c'4 r2.|\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("addVorschlag"), sheet::Event::Args({})));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Meta, FM_STRING("bd"), 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Note, fm::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N2p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::EOB));
}



BOOST_AUTO_TEST_CASE(test_SheetDefParser_vorschlag)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		I#4->I4 I4 I4 I4 |\n\
	}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_tie)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		c1~ | c1 \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::TiedNote, 0, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_documentUsingParser)
{
	using namespace fm;
	fm::String text = FM_STRING("\
using \"Chords1.chords\"; \n\
using \"Chords.chords\"; \n\
using \"simplePianoSheetTemplate.sheetTemplate\"; \n\
using \"chords/Chords1.chords\"; \n\
using \"chords/Chords.chords\"; \n\
using \"sheetTemplates/simplePianoSheetTemplate.sheetTemplate\"; \n\
using \"C:\\drivers\\Intel Rapid Storage Technology Driver\"; \n\
");
	sheet::compiler::SheetDefParser parser;
	auto doc = parser.parse(text);
	const auto &defs = doc.documentUsing;
	BOOST_CHECK(defs.usings.size() == 7);
	auto it = defs.usings.begin();
	BOOST_CHECK(*(it++) == FM_STRING("Chords1.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("Chords.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("simplePianoSheetTemplate.sheetTemplate"));
	BOOST_CHECK(*(it++) == FM_STRING("chords/Chords1.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("chords/Chords.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("sheetTemplates/simplePianoSheetTemplate.sheetTemplate"));
	BOOST_CHECK(*(it++) == FM_STRING("C:\\drivers\\Intel Rapid Storage Technology Driver"));

}

BOOST_AUTO_TEST_CASE(test_documentUsingParser_empty)
{
	using namespace fm;
	fm::String text = FM_STRING("\
\n\
	[\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
	{\n\
		f4 f4 f4 f4 | b4 b4 b4 b4 |\n\
	}\n\
	]\n\
    [\n\
	{\n\
		/ sheetTemplate: simplePianoSheetTemplate intro /\n\
		/ voicingStrategy : asNotated /\n\
		Cmaj | Cmaj C7 |\n\
	}\n\
	]\n\
\n\
");
	sheet::compiler::SheetDefParser parser;
	auto doc = parser.parse(text);
	BOOST_CHECK(doc.documentUsing.usings.size() == 0);

}


BOOST_AUTO_TEST_CASE(test_pitchmap_parser)
{
	using namespace fm;
	fm::String str(FM_STRING("\
\"bd\": c,,\n\
\"sn\": e,\n\
"));
	sheet::compiler::PitchmapParser parser;
	auto defs = parser.parse(str);
	BOOST_CHECK(defs.size() == 2);
	BOOST_CHECK(defs[0].name == FM_STRING("bd"));
	BOOST_CHECK(defs[0].pitch.pitch == fm::notes::C);
	BOOST_CHECK(defs[0].pitch.octave == -2);

	BOOST_CHECK(defs[1].name == FM_STRING("sn"));
	BOOST_CHECK(defs[1].pitch.pitch == fm::notes::E);
	BOOST_CHECK(defs[1].pitch.octave == -1);
}


BOOST_AUTO_TEST_CASE(test_trackinfo)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
type: notation; \n\
uname: bass; \n\
command: 01 02 03; \n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);

	BOOST_CHECK(defs.tracks[0].trackConfigs.size() == 3);

	BOOST_CHECK(defs.tracks[0].trackConfigs[0].name == FM_STRING("type") );
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args.size() == 1 );
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("notation") );

	BOOST_CHECK(defs.tracks[0].trackConfigs[1].name == FM_STRING("uname") );
	BOOST_CHECK(defs.tracks[0].trackConfigs[1].args.size() == 1 );
	BOOST_CHECK(defs.tracks[0].trackConfigs[1].args[0].value == FM_STRING("bass") );

	BOOST_CHECK(defs.tracks[0].trackConfigs[2].name == FM_STRING("command") );
	BOOST_CHECK(defs.tracks[0].trackConfigs[2].args.size() == 3 );
	BOOST_CHECK(defs.tracks[0].trackConfigs[2].args[0].value == FM_STRING("01") );
	BOOST_CHECK(defs.tracks[0].trackConfigs[2].args[1].value == FM_STRING("02") );
	BOOST_CHECK(defs.tracks[0].trackConfigs[2].args[2].value == FM_STRING("03") );
}



BOOST_AUTO_TEST_CASE(test_docmentInfo)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
instrumentDef: drums  SC1 9 0 32; \n\
command: 01 02 03; \n\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);

	BOOST_CHECK(defs.documentConfigs.size() == 2); 

	BOOST_CHECK(defs.documentConfigs.size() == 2);
	BOOST_CHECK(defs.documentConfigs[0].name == FM_STRING("instrumentDef") );
	BOOST_CHECK(defs.documentConfigs[0].args.size() == 5);
	BOOST_CHECK(defs.documentConfigs[0].args[0].value == FM_STRING("drums"));
	BOOST_CHECK(defs.documentConfigs[0].args[1].value == FM_STRING("SC1"));
	BOOST_CHECK(defs.documentConfigs[0].args[2].value == FM_STRING("9"));
	BOOST_CHECK(defs.documentConfigs[0].args[3].value == FM_STRING("0"));
	BOOST_CHECK(defs.documentConfigs[0].args[4].value == FM_STRING("32"));

	BOOST_CHECK(defs.documentConfigs[1].name == FM_STRING("command") );
	BOOST_CHECK(defs.documentConfigs[1].args.size() == 3);
	BOOST_CHECK(defs.documentConfigs[1].args[0].value == FM_STRING("01"));
	BOOST_CHECK(defs.documentConfigs[1].args[1].value == FM_STRING("02"));
	BOOST_CHECK(defs.documentConfigs[1].args[2].value == FM_STRING("03"));
}



BOOST_AUTO_TEST_CASE(test_tied_degree_failed)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		I1~ | I1 | \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::TiedDegree, fm::degrees::I, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Degree, fm::degrees::I, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::EOB));
}



BOOST_AUTO_TEST_CASE(test_event_positions)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
\n\
\n\
sheetInfo: xyz; \n\
[\n\
	-- a comment \n\
	-- a comment \n\
	trackConfig: xyz; \n\
\n\
	-- a comment \n\
	{\n\
		/ soundselect: 0 0/ -- another comment\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);

	// VOICE 1
	int idx = 0;
	auto ch = text[defs.tracks[0].voices[0].events[idx++].sourcePositionBegin];
	BOOST_CHECK( ch == '/' );
	ch = text[defs.tracks[0].voices[0].events[idx++].sourcePositionBegin];
	BOOST_CHECK( ch == '/' );
	auto event = defs.tracks[0].voices[0].events[idx++];
	BOOST_CHECK( text[event.sourcePositionBegin] == 'c' );

	event = defs.tracks[0].voices[0].events[idx++];
	ch = text[event.sourcePositionBegin];
	BOOST_CHECK( ch == 'd' );

	event = defs.tracks[0].voices[0].events[idx++];
	ch = text[event.sourcePositionBegin];
	BOOST_CHECK( ch == 'e' );

	event = defs.tracks[0].voices[0].events[idx++];
	ch = text[event.sourcePositionBegin];
	BOOST_CHECK( ch == 'f' );

	// TRACK & DOCUMENT
	ch = text[defs.tracks[0].voices[0].events[idx++].sourcePositionBegin];
	BOOST_CHECK( ch == '|' );	
	ch = text[defs.tracks[0].trackConfigs[0].sourcePositionBegin];
	BOOST_CHECK( ch == 't' );
	ch = text[defs.documentConfigs[0].sourcePositionBegin];
	BOOST_CHECK( ch == 's' );	
}

BOOST_AUTO_TEST_CASE(test_event_positions2)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
\n\
\n\
documentConfig: xyz; \n\
[\n\
	-- a comment \n\
	-- a comment \n\
	trackConfig: xyz; \n\
\n\
	-- a comment \n\
	{\n\
		c4 d4 r r f4 \"bd\"32 & && I <c d e>8 <I II II>16 \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);

	// VOICE 2
	size_t idx = 0;
	auto event = defs.tracks[0].voices[0].events[idx++];
	BOOST_CHECK( text[event.sourcePositionBegin] == 'c' );
	//BOOST_CHECK( (event.sourcePositionEnd - event.sourcePositionBegin) == 3 );
	BOOST_CHECK( text[event.sourcePositionEnd] == 'd' );

	event = defs.tracks[0].voices[0].events[idx++];
	//BOOST_CHECK( text[event.sourcePositionBegin] == 'I' );
	//BOOST_CHECK( (event.sourcePositionEnd - event.sourcePositionBegin) == 3 );
	//BOOST_CHECK( text[event.sourcePositionEnd] == 'V' );

	event = defs.tracks[0].voices[0].events[idx++];
	BOOST_CHECK( text[event.sourcePositionBegin] == 'r' );
	//BOOST_CHECK( (event.sourcePositionEnd - event.sourcePositionBegin) == 3 );
	BOOST_CHECK( text[event.sourcePositionEnd] == 'r' );

}

BOOST_AUTO_TEST_CASE(test_source_id)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("[\n\
	{\n\
		/ soundselect: 0 0/ -- another comment\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text, 101);
	for(const auto &event : defs.tracks[0].voices[0].events) {
		BOOST_CHECK( event.sourceId == 101 );
	}
	
}


BOOST_AUTO_TEST_CASE(test_x_repeat)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				c4 & & & | &1 \n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, fm::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_x_repeat_degree)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				I4 & & & | &1 \n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Degree, fm::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 4.0_N4));
}

BOOST_AUTO_TEST_CASE(test_x_tied_repeat)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				c4 &~ &~ &~ | &1 \n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, fm::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::TiedRepeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::TiedRepeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::TiedRepeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_x_vorschlag_repeat)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				c4 &->d4~ &~ | &1 \n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
}

BOOST_AUTO_TEST_CASE(test_x_repeat_degree_2)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		[\n\
			{\n\
				I4&&&|&1 \n\
			}\n\
		] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Degree, fm::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Repeat, sheet::PitchDef::NoPitch, 0, 4.0_N4));
}



BOOST_AUTO_TEST_CASE(test_degree_accendentials)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		I4 I# I## I### Ib Ibb Ibbb I#4 I##4 I###4 Ib4 Ibb4 Ibbb4 \n\
	}\n\
] \n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 13);
}

BOOST_AUTO_TEST_CASE(test_tags)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		\"tag1\"@c4 \"tag1 tag2\"@d4 \"tag1 tag2 tag3\"@ e4 \" tag1 tag2 \" @ f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::EOB));

	{
		const auto &ev = defs.tracks[0].voices[0].events[0];
		BOOST_CHECK(ev.tags.size() == 1);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[1];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[2];
		BOOST_CHECK(ev.tags.size() == 3);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag3")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[3];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
	}
}

BOOST_AUTO_TEST_CASE(test_tags_degree)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		\"tag1\"@I4 \"tag1 tag2\"@II4 \"tag1 tag2 tag3\"@ III4 \" tag1 tag2 \" @ IV4 | I'4 II'4 III'4 IV'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::Degree, fm::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::Degree, fm::degrees::II, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Degree, fm::degrees::III, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Degree, fm::degrees::IV, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Degree, fm::degrees::I, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::Degree, fm::degrees::II, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Degree, fm::degrees::III, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Degree, fm::degrees::IV, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::EOB));

	{
		const auto &ev = defs.tracks[0].voices[0].events[0];
		BOOST_CHECK(ev.tags.size() == 1);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[1];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[2];
		BOOST_CHECK(ev.tags.size() == 3);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag3")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[3];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(fm::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(fm::String("tag2")) != ev.tags.end() );
	}
}


BOOST_AUTO_TEST_CASE(test_ntolen)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		(c4 d16 e8)4 \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 1);
	const auto event = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK(event.type == sheet::Event::Group);
	BOOST_CHECK(event.duration == 1.0_N4);
	BOOST_CHECK(event.eventGroup.size() == 3);
	BOOST_CHECK(checkNote(event.eventGroup[0], sheet::Event::Note, fm::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(event.eventGroup[1], sheet::Event::Note, fm::notes::D, 0, 1.0_N16));
	BOOST_CHECK(checkNote(event.eventGroup[2], sheet::Event::Note, fm::notes::E, 0, 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_nested_ntolen)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		( (c d e)8 (f~ g a)16 (I~ II III)32 )4 \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 1);
	const auto &event = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK(event.type == sheet::Event::Group);
	BOOST_CHECK(event.duration == 1.0_N4);
	BOOST_CHECK(event.eventGroup.size() == 3);
	
	const auto &g1 = event.eventGroup[0];
	BOOST_CHECK(g1.eventGroup.size() == 3);
	BOOST_CHECK(g1.duration == 1.0_N8);
	BOOST_CHECK(checkNote(g1.eventGroup[0], sheet::Event::Note, fm::notes::C, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g1.eventGroup[1], sheet::Event::Note, fm::notes::D, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g1.eventGroup[2], sheet::Event::Note, fm::notes::E, 0, sheet::Event::NoDuration));

	const auto &g2 = event.eventGroup[1];
	BOOST_CHECK(g2.eventGroup.size() == 3);
	BOOST_CHECK(g2.duration == 1.0_N16);
	BOOST_CHECK(checkNote(g2.eventGroup[0], sheet::Event::TiedNote, fm::notes::F, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g2.eventGroup[1], sheet::Event::Note, fm::notes::G, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g2.eventGroup[2], sheet::Event::Note, fm::notes::A, 0, sheet::Event::NoDuration));

	const auto &g3 = event.eventGroup[2];
	BOOST_CHECK(g3.eventGroup.size() == 3);
	BOOST_CHECK(g3.duration == 1.0_N32);
	BOOST_CHECK(checkNote(g3.eventGroup[0], sheet::Event::TiedDegree, fm::degrees::I, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g3.eventGroup[1], sheet::Event::Degree, fm::degrees::II, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(g3.eventGroup[2], sheet::Event::Degree, fm::degrees::III, 0, sheet::Event::NoDuration));
}

BOOST_AUTO_TEST_CASE(test_issue_102_delimiter_for_meta_args)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	dconf: abc \"1 2 3\"; \n\
	[\n\
	tconf: abc 1 2 3; \n\
	{\n\
		/ metacommand: abc \"1 2 drei\"/\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("metacommand"), sheet::Event::Args({ makeArg("abc"), makeArg("1 2 drei") })));
	
}

BOOST_AUTO_TEST_CASE(test_repeats)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
| c d e f :|\n\
|: c d e f :| r1 \n\
:|: c d e f :|: r1 \n\
| c d e f g |^1 d e f g  |^2:  c d e f g :|^3: c d e f g :|^4: r1 |^5:\n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 46);
	const auto& events = defs.tracks[0].voices[0].events;
	BOOST_CHECK_EQUAL(events[5].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[5].stringValue, FM_STRING("__repeat_end_"));

	BOOST_CHECK_EQUAL(events[6].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[6].stringValue, FM_STRING("__repeat_begin_"));

	BOOST_CHECK_EQUAL(events[11].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[11].stringValue, FM_STRING("__repeat_end_"));

	BOOST_CHECK_EQUAL(events[13].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[13].stringValue, FM_STRING("__repeat_begin_and_end_"));

	BOOST_CHECK_EQUAL(events[18].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[18].stringValue, FM_STRING("__repeat_begin_and_end_"));

	BOOST_CHECK_EQUAL(events[26].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[26].stringValue.empty(), true);
	BOOST_CHECK_EQUAL(events[26].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[26].tags.begin()), FM_STRING("1"));

	BOOST_CHECK_EQUAL(events[31].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[31].stringValue, FM_STRING("__repeat_begin_"));
	BOOST_CHECK_EQUAL(events[31].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[31].tags.begin()), FM_STRING("2"));

	BOOST_CHECK_EQUAL(events[37].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[37].stringValue, FM_STRING("__repeat_begin_and_end_"));
	BOOST_CHECK_EQUAL(events[37].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[37].tags.begin()), FM_STRING("3"));	

	BOOST_CHECK_EQUAL(events[43].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[43].stringValue, FM_STRING("__repeat_begin_and_end_"));
	BOOST_CHECK_EQUAL(events[43].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[43].tags.begin()), FM_STRING("4"));

	BOOST_CHECK_EQUAL(events[45].type, sheet::Event::EOB);
	BOOST_CHECK_EQUAL(events[45].stringValue, FM_STRING("__repeat_begin_"));
	BOOST_CHECK_EQUAL(events[45].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[45].tags.begin()), FM_STRING("5"));
}

BOOST_AUTO_TEST_CASE(test_repeat_mark_fail_white_space)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^ 1: r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}


BOOST_AUTO_TEST_CASE(test_repeat_volta_fail_02)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^1 ^2: r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}
BOOST_AUTO_TEST_CASE(test_repeat_volta_fail_03)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^1 2: r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}


BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|:: c d e f | r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}

BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon_2)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f ::| r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}

BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon_3)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|:: r1 \n\
}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);

}