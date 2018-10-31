#include <boost/test/unit_test.hpp>
#include <iostream>
#include "compiler/parser.h"
#include <fm/literals.hpp>

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
	BOOST_CHECK( chordDefs.size() == 5 );
	auto &def = chordDefs[0];
	BOOST_CHECK( (chordDefs[0].name == FM_STRING("Xmaj")));
	auto interval = def.intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval == chordDefs[0].intervals.end()));


	BOOST_CHECK((chordDefs[1].name == FM_STRING("X7")));
	interval = chordDefs[1].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 10);
	BOOST_CHECK((interval == chordDefs[1].intervals.end()));


	BOOST_CHECK((chordDefs[2].name == FM_STRING("Xmaj7")));
	interval = chordDefs[2].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 11);
	BOOST_CHECK((interval == chordDefs[2].intervals.end()));

	BOOST_CHECK((chordDefs[3].name == FM_STRING("x7+")));
	interval = chordDefs[3].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 11);
	BOOST_CHECK((interval == chordDefs[3].intervals.end()));

	BOOST_CHECK((chordDefs[4].name == FM_STRING("X/V")));
	interval = chordDefs[4].intervals.begin();
	BOOST_CHECK((interval++)->value == -6);
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval == chordDefs[4].intervals.end()));
}

namespace {
	bool checkNote(const sheet::Event &ev, 
		sheet::Event::Type type,
		sheet::PitchDef::Pitch pitch = sheet::PitchDef::NoPitch,
		sheet::PitchDef::Octave octave = sheet::PitchDef::DefaultOctave,
		sheet::Event::Duration duration = sheet::Event::NoDuration) 
	{
		return ev.type == type
			&& ev.pitches.at(0).pitch == pitch
			&& ev.pitches.at(0).octave == octave
			&& ev.duration == duration;
	}
	bool checkNote(const sheet::Event &ev,
		sheet::Event::Type type,
		const sheet::Event::Pitches &pitches,
		sheet::Event::Duration duration = sheet::Event::NoDuration)
	{
		bool pre = ev.type == type && ev.duration == duration;
		if (!pre) {
			return false;
		}
		if (ev.pitches.size() != pitches.size()) {
			return false;
		}
		auto &it1 = ev.pitches.begin();
		auto &it2 = pitches.begin();
		while (it1 != ev.pitches.end())
		{
			if (it1->pitch != it2->pitch || it1->octave != it2->octave) {
				return false;
			}
			++it1;
			++it2;
		}
	}
}

BOOST_AUTO_TEST_CASE(test_styleDefparser_Simple)
{
	using namespace fm;
	fm::String text = FM_STRING("\
		@use anything; \n\
		@or not; \n\
	--some useless comment\n\
		section intro\n\
		[\n\
{I,4 II,,8 III,,,16 IV32 | I,4 I,, I,,, I | r1 | <I' III' V'>4 }\n\
{IV'4. VII''8. I'''16. II32. | II'4 II'' II''' II | r1 } \n\
] \n\
end\n\
");
	sheet::compiler::StyleDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.sections.size() == 1);
	BOOST_CHECK(defs.sections[0].name == FM_STRING("intro"));
	BOOST_CHECK(defs.sections[0].tracks.size() == 1);
	BOOST_CHECK(defs.sections[0].tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.sections[0].tracks[0].voices[0].events.size() == 13);
	BOOST_CHECK(defs.sections[0].tracks[0].voices[1].events.size() == 11);
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[0], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[1], sheet::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[2], sheet::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[3], sheet::Event::Degree, 4, 0, 1.0_N32));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[5], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[6], sheet::Event::Degree, 1, -2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[7], sheet::Event::Degree, 1, -3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[8], sheet::Event::Degree, 1, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));

	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[0], sheet::Event::Degree, 4, 1,  1.0_N4p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[1], sheet::Event::Degree, 7, 2, 1.0_N8p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[2], sheet::Event::Degree, 1, 3, 1.0_N16p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[3], sheet::Event::Degree, 2, 0, 1.0_N32p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[5], sheet::Event::Degree, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[6], sheet::Event::Degree, 2, 2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[7], sheet::Event::Degree, 2, 3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[8], sheet::Event::Degree, 2, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));
}