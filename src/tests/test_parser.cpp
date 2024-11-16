#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <com/literals.hpp>
#include <com/units.hpp>
#include "testhelper.h"

using namespace parser;

BOOST_AUTO_TEST_CASE(test_argQuoted)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: \"value\";		\
[{			    \
}]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
}

BOOST_AUTO_TEST_CASE(test_argNameQuotedValue)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: _name=\"value\";	 	\
[{			    \
}]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
	BOOST_CHECK(meta.args[0].name == "name");
}

BOOST_AUTO_TEST_CASE(test_argNameValue)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: _name=value;		\
[{			    \
}]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
	BOOST_CHECK(meta.args[0].name == "name");
}

BOOST_AUTO_TEST_CASE(test_argUnqouted)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: value;		\
[{			    \
}]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.documentConfigs.size() == 1); 
	auto const &meta = defs.documentConfigs[0];
	BOOST_CHECK(meta.args[0].value == "value");
}

BOOST_AUTO_TEST_CASE(test_argNameValue2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: _name=value _name2=value2;		\
[{			    \
}]			    \
");
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
key: myArpeggio direction down _style=legato2;		\
[{			    \
}]			    \
");
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[				\
key: \"value\";		\
{			    \
}				\
]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value") );
}

BOOST_AUTO_TEST_CASE(test_trackArgQuoted2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[				\
key: \"value two\";		\
{			    \
}				\
]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value two"));
}

BOOST_AUTO_TEST_CASE(test_trackArgUnquoted)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[				\
key: value;		\
{			    \
}				\
]			    \
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].trackConfigs[0].args[0].value == FM_STRING("value") );
}

BOOST_AUTO_TEST_CASE(test_chordDefparser)
{
	com::String str(FM_STRING("--here goes comment 1\n\
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

	ChordDefParser defParser;
	using namespace com::degrees;

	auto chordDefs = defParser.parse(str);
	BOOST_CHECK(chordDefs.size() == 5);
	auto &def = chordDefs[0];
	BOOST_CHECK((chordDefs[0].name == FM_STRING("Xmaj")));
	auto degreeDef = def.degreeDefs.begin();
	BOOST_CHECK((degreeDef)->degree == I);
	BOOST_CHECK((degreeDef++)->value == 1);

	BOOST_CHECK((degreeDef)->degree == III);
	BOOST_CHECK((degreeDef++)->value == 5);

	BOOST_CHECK((degreeDef)->degree == V);
	BOOST_CHECK((degreeDef++)->value == 8);
	BOOST_CHECK((degreeDef == chordDefs[0].degreeDefs.end()));


	BOOST_CHECK((chordDefs[1].name == FM_STRING("X7")));
	degreeDef = chordDefs[1].degreeDefs.begin();

	BOOST_CHECK((degreeDef)->degree == I);
	BOOST_CHECK((degreeDef++)->value == 1);

	BOOST_CHECK((degreeDef)->degree == III);
	BOOST_CHECK((degreeDef++)->value == 5);

	BOOST_CHECK((degreeDef)->degree == V);
	BOOST_CHECK((degreeDef++)->value == 8);

	BOOST_CHECK((degreeDef)->degree == VII);
	BOOST_CHECK((degreeDef++)->value == 10);
	BOOST_CHECK((degreeDef == chordDefs[1].degreeDefs.end()));


	BOOST_CHECK((chordDefs[2].name == FM_STRING("Xmaj7")));
	degreeDef = chordDefs[2].degreeDefs.begin();

	BOOST_CHECK((degreeDef)->degree == I);
	BOOST_CHECK((degreeDef++)->value == 1);

	BOOST_CHECK((degreeDef)->degree == III);
	BOOST_CHECK((degreeDef++)->value == 5);

	BOOST_CHECK((degreeDef)->degree == V);
	BOOST_CHECK((degreeDef++)->value == 8);

	BOOST_CHECK((degreeDef)->degree == VII);
	BOOST_CHECK((degreeDef++)->value == 11);
	BOOST_CHECK((degreeDef == chordDefs[2].degreeDefs.end()));

	BOOST_CHECK((chordDefs[3].name == FM_STRING("X7+")));
	degreeDef = chordDefs[3].degreeDefs.begin();

	BOOST_CHECK((degreeDef)->degree == I);
	BOOST_CHECK((degreeDef++)->value == 1);

	BOOST_CHECK((degreeDef)->degree == III);
	BOOST_CHECK((degreeDef++)->value == 5);

	BOOST_CHECK((degreeDef)->degree == V);
	BOOST_CHECK((degreeDef++)->value == 8);

	BOOST_CHECK((degreeDef)->degree == VII);
	BOOST_CHECK((degreeDef++)->value == 11);


	BOOST_CHECK((degreeDef == chordDefs[3].degreeDefs.end()));

	BOOST_CHECK((chordDefs[4].name == FM_STRING("X/V")));
	degreeDef = chordDefs[4].degreeDefs.begin();

	BOOST_CHECK((degreeDef)->degree == I);
	BOOST_CHECK((degreeDef++)->value == 1);

	BOOST_CHECK((degreeDef)->degree == III);
	BOOST_CHECK((degreeDef++)->value == 5);

	BOOST_CHECK((degreeDef)->degree == V);
	BOOST_CHECK((degreeDef++)->value == -6);


	BOOST_CHECK((degreeDef == chordDefs[4].degreeDefs.end()));
}

BOOST_AUTO_TEST_CASE(test_SheetDefParser)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 16);
	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 11);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Degree, 4, 0, 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::Degree, 1, -2, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Degree, 1, -3, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Degree, 1, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], documentModel::Event::Rest, documentModel::PitchDef::NoPitch, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[12], documentModel::Event::Degree, documentModel::Event::Pitches({ PitchDef(1, 1), PitchDef(3, 1), PitchDef(5, 1) }), 1.0_N4));

	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[13].metaArgs.size(), (size_t)1);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[13], FM_STRING("name"), documentModel::Event::Args({ makeArg("bass") })));

	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[14].metaArgs.size(), (size_t)2);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[14], FM_STRING("soundselect"), documentModel::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[15].metaArgs.size(), (size_t)2);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[15], FM_STRING("acommand"), documentModel::Event::Args({ makeArg("first", "arg1"), makeArg("second", "arg2") })));

	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], documentModel::Event::Degree, 4, 1, 1.0_N4p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], documentModel::Event::Degree, 7, 2, 1.0_N8p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], documentModel::Event::Degree, 1, 3, 1.0_N16p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], documentModel::Event::Degree, 2, 0, 1.0_N32p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[5], documentModel::Event::Degree, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[6], documentModel::Event::Degree, 2, 2, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[7], documentModel::Event::Degree, 2, 3, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[8], documentModel::Event::Degree, 2, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[9], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[10], documentModel::Event::Rest, documentModel::PitchDef::NoPitch, 0, 1.0_N1));
}


BOOST_AUTO_TEST_CASE(test_SheetDefParser_mixed_with_absolute_notes)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Degree, 4, 0, 1.0_N32));

	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 4);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], documentModel::Event::Note, com::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], documentModel::Event::Note, com::notes::D, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], documentModel::Event::Note, com::notes::E, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], documentModel::Event::Note, com::notes::F, 0, 1.0_N4));
}


BOOST_AUTO_TEST_CASE(test_alias_notes)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				\"bd\"4 \"sn\"8	<\"ht\" \"cymbal1\" >8\n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, FM_STRING("bd"), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Note, FM_STRING("sn"), 1.0_N8));
	BOOST_CHECK(defs.tracks[0].voices[0].events[2].pitches.size() == 2);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, documentModel::Event::Pitches({ PitchDef(FM_STRING("ht")), PitchDef(FM_STRING("cymbal1")) }), 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_alias_sheetdef)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				\"bd\"4 \"sn\"8	<\"ht\" \"cymbal1\" >8\n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, FM_STRING("bd"), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Note, FM_STRING("sn"), 1.0_N8));
	BOOST_CHECK(defs.tracks[0].voices[0].events[2].pitches.size() == 2);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, documentModel::Event::Pitches({ PitchDef(FM_STRING("ht")), PitchDef(FM_STRING("cymbal1")) }), 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_SheetDefParser_fail)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);
}


BOOST_AUTO_TEST_CASE(test_sheetDefParser)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
-- the documentModel, no tracks and voices here\n\
[\n\
type: documentModel;\n\
{\n\
	/ sheetTemplate : simplePianoSheetTemplate Intro / \n\
	/ voicingStrategy : asNotated / \n\
	Cmaj | Cmaj C7 | r G | A# B | C#maj79 r Ai \n\
}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 10);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), documentModel::Event::Args({  makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), documentModel::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], documentModel::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], documentModel::Event::EOB));

	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], documentModel::Event::Note, documentModel::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[5], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[6], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[7], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[8], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[9], documentModel::Event::EOB));

	const auto &chords = defs.tracks[1].voices[0].events;

	BOOST_CHECK(chords.size() == 16);
	BOOST_CHECK(checkMetaEvent(chords[0], FM_STRING("sheetTemplate"), documentModel::Event::Args({ makeArg("simplePianoSheetTemplate"), makeArg("Intro") })));
	BOOST_CHECK(checkMetaEvent(chords[1], FM_STRING("voicingStrategy"), documentModel::Event::Args({ makeArg("asNotated") })));
	BOOST_CHECK(checkChord(chords[2], FM_STRING("Cmaj")));
	BOOST_CHECK(checkNote(chords[3], documentModel::Event::EOB));
	BOOST_CHECK(checkChord(chords[4], FM_STRING("Cmaj")));
	BOOST_CHECK(checkChord(chords[5], FM_STRING("C7")));
	BOOST_CHECK(checkNote(chords[6], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(chords[7], documentModel::Event::Rest));
	BOOST_CHECK(checkChord(chords[8], FM_STRING("G")));
	BOOST_CHECK(checkNote(chords[9], documentModel::Event::EOB));
	BOOST_CHECK(checkChord(chords[10], FM_STRING("A#")));
	BOOST_CHECK(checkChord(chords[11], FM_STRING("B")));
	BOOST_CHECK(checkNote(chords[12], documentModel::Event::EOB));
	BOOST_CHECK(checkChord(chords[13], FM_STRING("C#maj79")));
	BOOST_CHECK(checkNote(chords[14], documentModel::Event::Rest));
	BOOST_CHECK(checkChord(chords[15], FM_STRING("Ai")));

	auto chordelements = chords[2].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::C);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("maj"));
	BOOST_CHECK(chords[2].chordDefName() == FM_STRING("Xmaj"));

	chordelements = chords[5].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::C);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("7"));
	BOOST_CHECK(chords[5].chordDefName() == FM_STRING("X7"));

	chordelements = chords[8].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::G);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[8].chordDefName() == FM_STRING("X"));

	chordelements = chords[10].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::AIS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING(""));
	BOOST_CHECK(chords[10].chordDefName() == FM_STRING("X"));

	chordelements = chords[13].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::CIS);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("maj79"));
	BOOST_CHECK(chords[13].chordDefName() == FM_STRING("Xmaj79"));

	chordelements = chords[15].chordElements();
	BOOST_CHECK(std::get<0>(chordelements) == com::notes::A);
	BOOST_CHECK(std::get<1>(chordelements) == FM_STRING("i"));
	BOOST_CHECK(chords[15].chordDefName() == FM_STRING("Xi"));

}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_02)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), documentModel::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), documentModel::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], documentModel::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], documentModel::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_03)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), documentModel::Event::Args({ makeArg("0"), makeArg("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), documentModel::Event::Args({ makeArg("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], documentModel::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], documentModel::Event::EOB));

	BOOST_CHECK(defs.tracks[1].voices.size() == 1);
	BOOST_CHECK(defs.tracks[1].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[0], documentModel::Event::Note, documentModel::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[1], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[2], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[3], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[5], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[6], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[7], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[8], documentModel::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[9], documentModel::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_expression_meta)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		c4\\pp d4 e4 f4 | \\ffff c'4 d'4 e'4 !p f'4 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 13);

	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, com::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("expression"), documentModel::Event::Args({ makeArg("pp") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, com::notes::D, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Note, com::notes::E, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::Note, com::notes::F, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::EOB));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[6], FM_STRING("expression"), documentModel::Event::Args({ makeArg("ffff") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Note, com::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Note, com::notes::D, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::Note, com::notes::E, 1, 1.0_N4));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[10], FM_STRING("expressionPlayedOnce"), documentModel::Event::Args({ makeArg("p") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], documentModel::Event::Note, com::notes::F, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[12], documentModel::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_vorschlag)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
	 d'32 -> c'4 r2.|\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("addVorschlag"), documentModel::Event::Args({})));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Meta, com::notes::D, 1, 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Note, com::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Rest, documentModel::PitchDef::NoPitch, 0, 1.0_N2p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::EOB));
}

BOOST_AUTO_TEST_CASE(test_alias_vorschlag)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
	 \"bd\"32->c'4 r2.|\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("addVorschlag"), documentModel::Event::Args({})));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Meta, FM_STRING("bd"), 1.0_N32));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Note, com::notes::C, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Rest, documentModel::PitchDef::NoPitch, 0, 1.0_N2p));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::EOB));
}



BOOST_AUTO_TEST_CASE(test_SheetDefParser_vorschlag)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		I#4->I4 I4 I4 I4 |\n\
	}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_tie)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		c1~ | c1 \n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::TiedNote, 0, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, 0, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_documentUsingParser)
{
	using namespace com;
	com::String text = FM_STRING("\
using \"Chords1.chords\"; \n\
using \"Chords.chords\"; \n\
using \"simplePianoSheetTemplate.sheetTemplate\"; \n\
using \"chords/Chords1.chords\"; \n\
using \"chords/Chords.chords\"; \n\
using \"sheetTemplates/simplePianoSheetTemplate.sheetTemplate\"; \n\
using \"C:\\drivers\\Intel Rapid Storage Technology Driver\"; \n\
");
	SheetDefParser parser;
	auto doc = parser.parse(text);
	const auto &defs = doc.documentUsings;
	BOOST_CHECK(defs.size() == 7);
	auto it = defs.begin();
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
	using namespace com;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
	auto doc = parser.parse(text);
	BOOST_CHECK(doc.documentUsings.size() == 0);

}


BOOST_AUTO_TEST_CASE(test_pitchmap_parser)
{
	using namespace com;
	com::String str(FM_STRING("\
\"bd\": c,,\n\
\"sn\": e,\n\
"));
	PitchmapParser parser;
	auto defs = parser.parse(str);
	BOOST_CHECK(defs.size() == 2);
	BOOST_CHECK(defs[0].name == FM_STRING("bd"));
	BOOST_CHECK(defs[0].pitch.pitch == com::notes::C);
	BOOST_CHECK(defs[0].pitch.octave == -2);

	BOOST_CHECK(defs[1].name == FM_STRING("sn"));
	BOOST_CHECK(defs[1].pitch.pitch == com::notes::E);
	BOOST_CHECK(defs[1].pitch.octave == -1);
}


BOOST_AUTO_TEST_CASE(test_trackinfo)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		I1~ | I1 | \n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 4);

	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::TiedDegree, com::degrees::I, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Degree, com::degrees::I, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::EOB));
}



BOOST_AUTO_TEST_CASE(test_event_positions)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
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
	SheetDefParser parser;
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
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("[\n\
	{\n\
		/ soundselect: 0 0/ -- another comment\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | \n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text, 101);
	for(const auto &event : defs.tracks[0].voices[0].events) {
		BOOST_CHECK( event.sourceId == 101 );
	}
	
}


BOOST_AUTO_TEST_CASE(test_x_repeat)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				c4 & & & | &1 \n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, com::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_x_repeat_degree)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				I4 & & & | &1 \n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 4.0_N4));
}

BOOST_AUTO_TEST_CASE(test_x_tied_repeat)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				c4 &~ &~ &~ | &1 \n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, com::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::TiedRepeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::TiedRepeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::TiedRepeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_x_vorschlag_repeat)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				c4 &->d4~ &~ | &1 \n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
}

BOOST_AUTO_TEST_CASE(test_x_repeat_degree_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
		[\n\
			{\n\
				I4&&&|&1 \n\
			}\n\
		] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 6);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 0));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Repeat, documentModel::PitchDef::NoPitch, 0, 4.0_N4));
}



BOOST_AUTO_TEST_CASE(test_degree_accendentials)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		I4 I# I## I### Ib Ibb Ibbb I#4 I##4 I###4 Ib4 Ibb4 Ibbb4 \n\
	}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 13);
}

BOOST_AUTO_TEST_CASE(test_tags)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		\"tag1\"@c4 \"tag1 tag2\"@d4 \"tag1 tag2 tag3\"@ e4 \" tag1 tag2 \" @ f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::EOB));

	{
		const auto &ev = defs.tracks[0].voices[0].events[0];
		BOOST_CHECK(ev.tags.size() == 1);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[1];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[2];
		BOOST_CHECK(ev.tags.size() == 3);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag3")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[3];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
	}
}

BOOST_AUTO_TEST_CASE(test_tags_degree)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		\"tag1\"@I4 \"tag1 tag2\"@II4 \"tag1 tag2 tag3\"@ III4 \" tag1 tag2 \" @ IV4 | I'4 II'4 III'4 IV'4 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::I, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Degree, com::degrees::II, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Degree, com::degrees::III, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Degree, com::degrees::IV, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], documentModel::Event::Degree, com::degrees::I, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], documentModel::Event::Degree, com::degrees::II, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], documentModel::Event::Degree, com::degrees::III, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], documentModel::Event::Degree, com::degrees::IV, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], documentModel::Event::EOB));

	{
		const auto &ev = defs.tracks[0].voices[0].events[0];
		BOOST_CHECK(ev.tags.size() == 1);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[1];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[2];
		BOOST_CHECK(ev.tags.size() == 3);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag3")) != ev.tags.end() );
	}
	{
		const auto &ev = defs.tracks[0].voices[0].events[3];
		BOOST_CHECK(ev.tags.size() == 2);
		BOOST_CHECK( ev.tags.find(com::String("tag1")) != ev.tags.end() );
		BOOST_CHECK( ev.tags.find(com::String("tag2")) != ev.tags.end() );
	}
}


BOOST_AUTO_TEST_CASE(test_ntolen)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		(c4 d16 e8)4 \n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 1);
	const auto event = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK(event.type == documentModel::Event::Group);
	BOOST_CHECK(event.duration == 1.0_N4);
	BOOST_CHECK(event.eventGroup.size() == 3);
	BOOST_CHECK(checkNote(event.eventGroup[0], documentModel::Event::Note, com::notes::C, 0, 1.0_N4));
	BOOST_CHECK(checkNote(event.eventGroup[1], documentModel::Event::Note, com::notes::D, 0, 1.0_N16));
	BOOST_CHECK(checkNote(event.eventGroup[2], documentModel::Event::Note, com::notes::E, 0, 1.0_N8));
}

BOOST_AUTO_TEST_CASE(test_nested_ntolen)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		( (c d e)8 (f~ g a)16 (I~ II III)32 )4 \n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 1);
	const auto &event = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK(event.type == documentModel::Event::Group);
	BOOST_CHECK(event.duration == 1.0_N4);
	BOOST_CHECK(event.eventGroup.size() == 3);
	
	const auto &g1 = event.eventGroup[0];
	BOOST_CHECK(g1.eventGroup.size() == 3);
	BOOST_CHECK(g1.duration == 1.0_N8);
	BOOST_CHECK(checkNote(g1.eventGroup[0], documentModel::Event::Note, com::notes::C, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g1.eventGroup[1], documentModel::Event::Note, com::notes::D, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g1.eventGroup[2], documentModel::Event::Note, com::notes::E, 0, documentModel::Event::NoDuration));

	const auto &g2 = event.eventGroup[1];
	BOOST_CHECK(g2.eventGroup.size() == 3);
	BOOST_CHECK(g2.duration == 1.0_N16);
	BOOST_CHECK(checkNote(g2.eventGroup[0], documentModel::Event::TiedNote, com::notes::F, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g2.eventGroup[1], documentModel::Event::Note, com::notes::G, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g2.eventGroup[2], documentModel::Event::Note, com::notes::A, 0, documentModel::Event::NoDuration));

	const auto &g3 = event.eventGroup[2];
	BOOST_CHECK(g3.eventGroup.size() == 3);
	BOOST_CHECK(g3.duration == 1.0_N32);
	BOOST_CHECK(checkNote(g3.eventGroup[0], documentModel::Event::TiedDegree, com::degrees::I, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g3.eventGroup[1], documentModel::Event::Degree, com::degrees::II, 0, documentModel::Event::NoDuration));
	BOOST_CHECK(checkNote(g3.eventGroup[2], documentModel::Event::Degree, com::degrees::III, 0, documentModel::Event::NoDuration));
}

BOOST_AUTO_TEST_CASE(test_issue_102_delimiter_for_meta_args)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	dconf: abc \"1 2 3\"; \n\
	[\n\
	tconf: abc 1 2 3; \n\
	{\n\
		/ metacommand: abc \"1 2 drei\"/\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("metacommand"), documentModel::Event::Args({ makeArg("abc"), makeArg("1 2 drei") })));
	
}

BOOST_AUTO_TEST_CASE(test_repeats)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
| c d e f :|\n\
|: c d e f :| r1 \n\
:|: c d e f :|: r1 \n\
| c d e f g |^1 d e f g  |^2:  c d e f g :|^3: c d e f g :|^4: r1 |^5:\n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 46);
	const auto& events = defs.tracks[0].voices[0].events;
	BOOST_CHECK_EQUAL(events[5].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[5].stringValue, FM_STRING("__repeat_end_"));

	BOOST_CHECK_EQUAL(events[6].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[6].stringValue, FM_STRING("__repeat_begin_"));

	BOOST_CHECK_EQUAL(events[11].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[11].stringValue, FM_STRING("__repeat_end_"));

	BOOST_CHECK_EQUAL(events[13].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[13].stringValue, FM_STRING("__repeat_begin_and_end_"));

	BOOST_CHECK_EQUAL(events[18].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[18].stringValue, FM_STRING("__repeat_begin_and_end_"));

	BOOST_CHECK_EQUAL(events[26].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[26].stringValue.empty(), true);
	BOOST_CHECK_EQUAL(events[26].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[26].tags.begin()), FM_STRING("1"));

	BOOST_CHECK_EQUAL(events[31].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[31].stringValue, FM_STRING("__repeat_begin_"));
	BOOST_CHECK_EQUAL(events[31].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[31].tags.begin()), FM_STRING("2"));

	BOOST_CHECK_EQUAL(events[37].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[37].stringValue, FM_STRING("__repeat_begin_and_end_"));
	BOOST_CHECK_EQUAL(events[37].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[37].tags.begin()), FM_STRING("3"));	

	BOOST_CHECK_EQUAL(events[43].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[43].stringValue, FM_STRING("__repeat_begin_and_end_"));
	BOOST_CHECK_EQUAL(events[43].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[43].tags.begin()), FM_STRING("4"));

	BOOST_CHECK_EQUAL(events[45].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[45].stringValue, FM_STRING("__repeat_begin_"));
	BOOST_CHECK_EQUAL(events[45].tags.size(), 1);
	BOOST_CHECK_EQUAL(*(events[45].tags.begin()), FM_STRING("5"));
}

BOOST_AUTO_TEST_CASE(test_repeat_mark_fail_white_space)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^ 1: r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}


BOOST_AUTO_TEST_CASE(test_repeat_volta_fail_02)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^1 ^2: r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}
BOOST_AUTO_TEST_CASE(test_repeat_volta_fail_03)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|^1 2: r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}


BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|:: c d e f | r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f ::| r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_repeats_fail_duplicated_multiplier_colon_3)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :|:: r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_extended_note_events)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
	h i j k l m n o p q s u v w x y z \n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 17);
	const auto& events = defs.tracks[0].voices[0].events;
	int c = 0;
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("h"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("i"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("j"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("k"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("l"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("m"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("n"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("o"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("p"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("q"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("s"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("u"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("v"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("w"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("x"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("y"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("z"));
}

BOOST_AUTO_TEST_CASE(test_extended_note_events_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
	hijklmnopqsuvwxyz\n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 17);
	const auto& events = defs.tracks[0].voices[0].events;
	int c = 0;
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("h"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("i"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("j"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("k"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("l"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("m"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("n"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("o"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("p"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("q"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("s"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("u"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("v"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("w"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("x"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("y"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("z"));
}

BOOST_AUTO_TEST_CASE(test_extended_note_events_octaves)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
	h' i'' j''' k'''' l''''' m, n,, o,,, p,,,, q,,,,, s' u' v' w' x' y' z' \n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 17);
	const auto& events = defs.tracks[0].voices[0].events;
	int c = 0;
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("h'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("i''"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("j'''"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("k''''"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("l'''''"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("m,"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("n,,"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("o,,,"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("p,,,,"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("q,,,,,"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("s'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("u'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("v'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("w'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("x'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("y'"));
	BOOST_CHECK_EQUAL(events[c].type, documentModel::Event::Note);
	BOOST_CHECK_EQUAL(events[c++].pitches[0].alias, com::String("z'"));
}

BOOST_AUTO_TEST_CASE(test_chordDefAdjunctDegrees)
{
	com::String str(FM_STRING("\
Xmaj: I=1 (II=3) III=4 (IV = 5) V=7 (VI=9) (VII=11)\n\
"));

	ChordDefParser defParser;
	using namespace com::degrees;

	auto chordDefs = defParser.parse(str);
	BOOST_CHECK(chordDefs.size() == 1);
	auto& def = chordDefs[0];
	BOOST_CHECK((chordDefs[0].name == FM_STRING("Xmaj")));
	BOOST_CHECK_EQUAL(def.degreeDefs.size(), size_t(7));
	auto degreeDef = def.degreeDefs.begin();
	BOOST_CHECK_EQUAL((degreeDef)->degree, I);
	BOOST_CHECK_EQUAL((degreeDef)->value, 1);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, false);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, II);
	BOOST_CHECK_EQUAL((degreeDef)->value, 3);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, true);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, III);
	BOOST_CHECK_EQUAL((degreeDef)->value, 4);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, false);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, IV);
	BOOST_CHECK_EQUAL((degreeDef)->value, 5);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, true);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, V);
	BOOST_CHECK_EQUAL((degreeDef)->value, 7);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, false);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, VI);
	BOOST_CHECK_EQUAL((degreeDef)->value, 9);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, true);
	++degreeDef;
	BOOST_CHECK_EQUAL((degreeDef)->degree, VII);
	BOOST_CHECK_EQUAL((degreeDef)->value, 11);
	BOOST_CHECK_EQUAL((degreeDef)->isAdjunct, true);
}

BOOST_AUTO_TEST_CASE(test_ForceAdjunctDegree)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		I4 !VI4 ! VI, !VI# !VI#, <I !VI>4 \n\
	}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(6));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::I, 0, 1.0_N4));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].pitches[0].forceDegree, false);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], documentModel::Event::Degree, com::degrees::VI, 0, 1.0_N4));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[1].pitches[0].forceDegree, true);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], documentModel::Event::Degree, com::degrees::VI, -1, 0.0));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[2].pitches[0].forceDegree, true);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], documentModel::Event::Degree, com::degrees::VIis, 0, 0.0));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[3].pitches[0].forceDegree, true);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], documentModel::Event::Degree, com::degrees::VIis, -1, 0.0));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[4].pitches[0].forceDegree, true);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[5].type, documentModel::Event::Degree);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[5].pitches[0].pitch, com::degrees::I);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[5].pitches[0].forceDegree, false);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[5].pitches[1].pitch, com::degrees::VI);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[5].pitches[1].forceDegree, true);
}

BOOST_AUTO_TEST_CASE(test_ForceAdjunctDegree_ExpressionExclamationMarkIssue)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		VI !p\n\
	}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::VI, 0, 0));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].pitches[0].forceDegree, false);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[1].type, documentModel::Event::Meta);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[1].stringValue, com::String("expressionPlayedOnce"));
}

BOOST_AUTO_TEST_CASE(test_ForceAdjunctDegree_ExclamationMarkWhitespaceIssue)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		VI!p\n\
	}\n\
] \n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], documentModel::Event::Degree, com::degrees::VI, 0, 0));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].pitches[0].forceDegree, false);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[1].type, documentModel::Event::Meta);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[1].stringValue, com::String("expressionPlayedOnce"));
}

BOOST_AUTO_TEST_CASE(test_347_repeat_more_than_once)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
 c d e f :(x2)|\n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 5);
	const auto& events = defs.tracks[0].voices[0].events;
	BOOST_CHECK_EQUAL(events[4].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[4].numberOfRepeats, int(2));
	BOOST_CHECK_EQUAL(events[4].stringValue, FM_STRING("__repeat_end_"));
}

BOOST_AUTO_TEST_CASE(test_347_repeat_more_than_once_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
 c d e f :(x2)|: c d e f :|\n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 10);
	const auto& events = defs.tracks[0].voices[0].events;
	BOOST_CHECK_EQUAL(events[4].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[4].numberOfRepeats, int(2));
	BOOST_CHECK_EQUAL(events[4].stringValue, FM_STRING("__repeat_begin_and_end_"));
}

BOOST_AUTO_TEST_CASE(test_347_repeat_more_than_once_default_value)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
 c d e f :|\n\
}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), 5);
	const auto& events = defs.tracks[0].voices[0].events;
	BOOST_CHECK_EQUAL(events[4].type, documentModel::Event::EOB);
	BOOST_CHECK_EQUAL(events[4].numberOfRepeats, int(0));
	BOOST_CHECK_EQUAL(events[4].stringValue, FM_STRING("__repeat_end_"));
}

BOOST_AUTO_TEST_CASE(test_347_fail_duplicated_multiplier_colon)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f :(x2):| r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_347_fail_duplicated_multiplier_colon_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|: c d e f ::(x2)| r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_347_fail_wrong_position)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\n\
[\n\
{\n\
|(x2): c d e f :| r1 \n\
}\n\
]\n\
");
	SheetDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);

}

BOOST_AUTO_TEST_CASE(test_annotations_for_tuplets_1)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		\"myTag\"@(c d e f g)1 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 2);
	const auto &ev = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK_EQUAL(ev.tags.size(), size_t(1));
	BOOST_CHECK( ev.tags.find(com::String("myTag")) != ev.tags.end() );
}

BOOST_AUTO_TEST_CASE(test_annotations_for_tuplets_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		(c \"myTag\"@d e f g)1 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 2);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].eventGroup.size(), size_t(5));
	const auto &ev = defs.tracks[0].voices[0].events[0].eventGroup[1];
	BOOST_CHECK_EQUAL(ev.tags.size(), size_t(1));
	BOOST_CHECK( ev.tags.find(com::String("myTag")) != ev.tags.end() );
}

BOOST_AUTO_TEST_CASE(test_phrase_def)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
myPhrase = c d e f;\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).name, com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).type, documentModel::DocumentConfig::TypePhraseDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(0).pitches.front().pitch, com::notes::C);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(1).pitches.front().pitch, com::notes::D);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(2).pitches.front().pitch, com::notes::E);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(3).pitches.front().pitch, com::notes::F);
}

BOOST_AUTO_TEST_CASE(test_phrase_def_with_docdefs)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
myDocDef: arg1 arg2;\n\
myPhrase = c4 d e f;\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).args.size(), size_t(2));
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).name, com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).type, documentModel::DocumentConfig::TypePhraseDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(0).pitches.front().pitch, com::notes::C);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(1).pitches.front().pitch, com::notes::D);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(2).pitches.front().pitch, com::notes::E);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(3).pitches.front().pitch, com::notes::F);
}

BOOST_AUTO_TEST_CASE(test_phrase_def_before_docdefs)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
myPhrase = c d e f;\n\
myDocDef: arg1 arg2;\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).name, com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).type, documentModel::DocumentConfig::TypePhraseDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(0).pitches.front().pitch, com::notes::C);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(1).pitches.front().pitch, com::notes::D);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(2).pitches.front().pitch, com::notes::E);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).events.at(3).pitches.front().pitch, com::notes::F);
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).args.size(), size_t(2));
}

BOOST_AUTO_TEST_CASE(test_phrase_def_between_docdefs)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
myDocDef: arg1 arg2;\n\
myPhrase = c d e f;\n\
myDocDef: arg1 arg2;\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(3));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).args.size(), size_t(2));
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).name, com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).type, documentModel::DocumentConfig::TypePhraseDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(0).pitches.front().pitch, com::notes::C);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(1).pitches.front().pitch, com::notes::D);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(2).pitches.front().pitch, com::notes::E);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(3).pitches.front().pitch, com::notes::F);
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).args.size(), size_t(2));
}

BOOST_AUTO_TEST_CASE(test_phrase_def_between_docdefs_degrees)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
myDocDef: arg1 arg2;\n\
myPhrase = I II III IV;\n\
myDocDef: arg1 arg2;\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(3));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(0).args.size(), size_t(2));
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).name, com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).type, documentModel::DocumentConfig::TypePhraseDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(0).pitches.front().pitch, com::degrees::I);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(1).pitches.front().pitch, com::degrees::II);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(2).pitches.front().pitch, com::degrees::III);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(1).events.at(3).pitches.front().pitch, com::degrees::IV);
	//
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).name, com::String("myDocDef"));
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).type, documentModel::DocumentConfig::TypeConfigDef);
	BOOST_CHECK_EQUAL(defs.documentConfigs.at(2).args.size(), size_t(2));
}

BOOST_AUTO_TEST_CASE(test_use_phrase)
{
	using namespace com;
	using namespace documentModel;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		>\"myPhrase\"8 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].type, Event::Phrase);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].phraseName(), com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].isPhraseStrechedPlayback, true);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].duration, 1.0_N8);
}
BOOST_AUTO_TEST_CASE(test_use_tied_phrase)
{
	using namespace com;
	using namespace documentModel;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		>\"myPhrase\"8~ |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].type, Event::TiedPhrase);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].phraseName(), com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].duration, 1.0_N8);
}

BOOST_AUTO_TEST_CASE(test_phrase_with_annotations)
{
	using namespace com;
	using namespace documentModel;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		\"myTag1 myTag2\"@>\"myPhrase\"8 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].type, Event::Phrase);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].phraseName(), com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].duration, 1.0_N8);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].tags.size(), size_t(2));
	const auto &ev = defs.tracks[0].voices[0].events[0];
	BOOST_CHECK( ev.tags.find(com::String("myTag1")) != ev.tags.end() );
	BOOST_CHECK( ev.tags.find(com::String("myTag2")) != ev.tags.end() );
}

BOOST_AUTO_TEST_CASE(order_of_sheet_components)
{
	using namespace com;
	using namespace documentModel;
	using documentModel::PitchDef;
	com::String usings = FM_STRING("\
using \"myFile.template\";\
");
	com::String phrase = FM_STRING("\
myPhrase = c d e f;	\
");
	com::String track = FM_STRING("\
[  \
{	\
	c d e f	\
}	\
]	\
");
	{
		SheetDefParser parser;
		auto text = usings + phrase + track;
		auto defs = parser.parse(text);
		BOOST_CHECK_EQUAL(defs.documentUsings.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	}
	{
		SheetDefParser parser;
		auto text = usings + track + phrase;
		auto defs = parser.parse(text);
		BOOST_CHECK_EQUAL(defs.documentUsings.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	}
	{
		SheetDefParser parser;
		auto text = track + usings + phrase;
		auto defs = parser.parse(text);
		BOOST_CHECK_EQUAL(defs.documentUsings.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	}
	{
		SheetDefParser parser;
		auto text = track + phrase + usings;
		auto defs = parser.parse(text);
		BOOST_CHECK_EQUAL(defs.documentUsings.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(1));
		BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(1));
	}
	{
		SheetDefParser parser;
		auto text = track + usings + phrase + usings + track + phrase;
		auto defs = parser.parse(text);
		BOOST_CHECK_EQUAL(defs.documentUsings.size(), size_t(2));
		BOOST_CHECK_EQUAL(defs.documentConfigs.size(), size_t(2));
		BOOST_CHECK_EQUAL(defs.tracks.size(), size_t(2));
	}
}


BOOST_AUTO_TEST_CASE(test_use_phrase_normal_mode)
{
	using namespace com;
	using namespace documentModel;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
[\n\
	{\n\
		>> \"myPhrase\"8 |\n\
	}\n\
]\n\
");
	SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].type, Event::Phrase);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].phraseName(), com::String("myPhrase"));
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].isPhraseStrechedPlayback, false);
	BOOST_CHECK_EQUAL(defs.tracks[0].voices[0].events[0].duration, 1.0_N8);
}