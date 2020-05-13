#include <boost/test/unit_test.hpp>
#include <fm/common.hpp>
#include <fm/midi.hpp>
#include <fm/werckmeister.hpp>
#include <iterator>
#include <sheet/tools.h>
#include <sheet/Argument.h>

BOOST_AUTO_TEST_CASE(test_getArgValue)
{
	std::vector<sheet::Argument> args = {
		{fm::String("1"), fm::String("name1")},
		{fm::String("2"), fm::String("")},
		{fm::String("3"), fm::String("name3")}
	};
	BOOST_CHECK_EQUAL(sheet::getArgValue<int>(args, "name1", 0), 1);
	BOOST_CHECK_EQUAL(sheet::getArgValue<int>(args, "name2", 1), 2);
	BOOST_CHECK_EQUAL(sheet::getArgValue<int>(args, "noname", 9, 101), 101);
	BOOST_CHECK_THROW(sheet::getArgValue<int>(args, "noname", 9), fm::Exception);
}

BOOST_AUTO_TEST_CASE(test_endswap)
{
	using namespace fm;
	if (isLittleEndian()) {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_CHECK(bytes[0] == 0xEF);
		BOOST_CHECK(bytes[1] == 0xCD);
		BOOST_CHECK(bytes[2] == 0xAB);
		BOOST_CHECK(bytes[3] == 0xAA);
		endswap(&x);
		BOOST_CHECK(bytes[0] == 0xAA);
		BOOST_CHECK(bytes[1] == 0xAB);
		BOOST_CHECK(bytes[2] == 0xCD);
		BOOST_CHECK(bytes[3] == 0xEF);
	}
	else {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_CHECK(bytes[0] == 0xAA);
		BOOST_CHECK(bytes[1] == 0xAB);
		BOOST_CHECK(bytes[2] == 0xCD);
		BOOST_CHECK(bytes[3] == 0xEF);
		endswap(&x);
		BOOST_CHECK(bytes[0] == 0xEF);
		BOOST_CHECK(bytes[1] == 0xCD);
		BOOST_CHECK(bytes[2] == 0xAB);
		BOOST_CHECK(bytes[3] == 0xAA);
	}
}

BOOST_AUTO_TEST_CASE(test_resource_loader)
{
#ifdef WIN32
	auto resource = fm::getWerckmeister().openResource(FM_STRING("../../examples/chords/default.chords"));
#else
	auto resource = fm::getWerckmeister().openResource(FM_STRING("../examples/chords/default.chords"));
#endif
	fm::StreamBuffIterator eos;
	fm::StreamBuffIterator it(*resource.get());
	fm::String res(it, eos);
	BOOST_CHECK(res.length() > 0);
}


BOOST_AUTO_TEST_CASE(test_get_line_and_position_1)
{
	fm::String source = FM_STRING("-- document configs\n\
@using 'Chords1.chdef';\n\
@using 'simplePianoStyle.style';\n\
\n\
	[-- xyz\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
\n\
\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	[{\n\
		/ style: simplePianoStyle:intro /\n\
			/ voicingStrategy : asNotated /\n\
			Cmaj | Cmaj C7 |\n\
	}]\n\
		[{}]");
	
	fm::String line;
	int position = -1;
	std::tie(line, position) = sheet::getLineAndPosition<fm::String>(source, 149);
	line[position] = 'X';
	fm::String expected = FM_STRING("c4 d4 e4 f4 | X4 d4 e4 f4 |");
	BOOST_CHECK( line == expected );
}


BOOST_AUTO_TEST_CASE(test_get_line_and_position_12)
{
	std::string source = "-- document configs\n\
@using 'Chords1.chdef';\n\
@using 'simplePianoStyle.style';\n\
\n\
	[-- xyz\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
\n\
\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	[{\n\
		/ style: simplePianoStyle:intro /\n\
			/ voicingStrategy : asNotated /\n\
			Cmaj | Cmaj C7 |\n\
	}]\n\
		[{}]";
	
	std::string line;
	int position = -1;
	std::tie(line, position) = sheet::getLineAndPosition<std::string>(source, 149);
	line[position] = 'X';
	std::string expected = "c4 d4 e4 f4 | X4 d4 e4 f4 |";
	BOOST_CHECK( line == expected );
}

BOOST_AUTO_TEST_CASE(test_get_line_and_position_2)
{
	fm::String source = FM_STRING("-- document configs\n\
@using 'Chords1.chdef';\n\
@using 'simplePianoStyle.style';\n\
\n\
	[-- xyz\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
\n\
\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	[{\n\
		/ style: simplePianoStyle:intro /\n\
			/ voicingStrategy : asNotated /\n\
			Cmaj | Cmaj C7 |\n\
	}]\n\
		[{}]");
	
	fm::String line;
	int position = -1;
	std::tie(line, position) = sheet::getLineAndPosition(source, 0);
	line[position] = 'X';
	fm::String expected = FM_STRING("X- document configs");
	BOOST_CHECK( line == expected );
}

BOOST_AUTO_TEST_CASE(test_get_line_and_position_3)
{
	fm::String source = FM_STRING("-- document configs\n\
@using 'Chords1.chdef';\n\
@using 'simplePianoStyle.style';\n\
\n\
	[-- xyz\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
\n\
\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	[{\n\
		/ style: simplePianoStyle:intro /\n\
			/ voicingStrategy : asNotated /\n\
			Cmaj | Cmaj C7 |\n\
	}]\n\
		[{}]");
	
	fm::String line;
	int position = -1;
	std::tie(line, position) = sheet::getLineAndPosition(source, source.length()-1);
	line[position] = 'X';
	fm::String expected = FM_STRING("[{}X");
	BOOST_CHECK( line == expected );
}

BOOST_AUTO_TEST_CASE(remove_comments_0)
{
	fm::String source = FM_STRING("");
	fm::String excpected = FM_STRING("");
	sheet::removeComments(source.begin(), source.end());
	BOOST_CHECK(source == excpected);

}
BOOST_AUTO_TEST_CASE(remove_comments_1)
{
	fm::String source = FM_STRING("--abc");
	fm::String excpected = FM_STRING("     ");
	sheet::removeComments(source.begin(), source.end());
	BOOST_CHECK(source == excpected);
}

BOOST_AUTO_TEST_CASE(remove_comments_2)
{
	fm::String source = FM_STRING("abc--def");
	fm::String excpected = FM_STRING("abc     ");
	sheet::removeComments(source.begin(), source.end());
	BOOST_CHECK(source == excpected);
}
BOOST_AUTO_TEST_CASE(remove_comments_3)
{
	fm::String source = FM_STRING("\n\
	\n\
	\n\
	abc\n\
	-- abc\n\
	def");
	fm::String excpected = FM_STRING("\n\
	\n\
	\n\
	abc\n\
	      \n\
	def");
	sheet::removeComments(source.begin(), source.end());
	BOOST_CHECK(source == excpected);
	//std::wcout << source << std::endl;
}




BOOST_AUTO_TEST_CASE(position_to_row_and_column_ipanema)
{
	std::string source = "using \"./chords/default.chords\";\nusing \"./styles/bossanova.style\";\nusing \"./pitchmaps/defaultMidiDrumMap.pitchmap\";\n--using \"./pitchmaps/battery3.tight.pitchmap\";\n\ntempo: 140;\ndevice: SC1 midi 2;\n--device: BT midi 8;\n\n--             instrument dv ch cc pc\ninstrumentDef: piano      SC1 2 0 0;\ninstrumentDef: lead     SC1 3 0 11;\ninstrumentDef: bass    SC1 7 8 33;\ninstrumentDef: drums   SC1 9 0 32;\ninstrumentDef: guitar  SC1 6 0 24;\n\ninstrumentConf: guitar volume 80 pan 20;\ninstrumentConf: piano volume 70 pan 80;\ninstrumentConf: lead volume 80;\ninstrumentConf: bass volume 70;\ninstrumentConf: drums volume 80;\n\n[\ninstrument: lead;\n{\n    \\ffff\n    g4. e8 e d4 g8~  | g4 e8 e4 e8 d g~ | g4 e e d8 g~ | g8 g e e4 e8 d f~ |\n    f8 d4 d4 d8 c e~ | e c4 c4 c8 bes,4 | r4 c2.~ | c1 |\n    g4. e8 e d4 g8~  | g4 e8 e4 e8 d g~ | g4 e e d8 g~ | g8 g e e4 e8 d f~ |\n    f8 d4 d4 d8 c e~ | e c4 c4 c8 bes,4 | \n    r4 c2.~ | c2. r4 | f1~ | f4t ges f es f es |\n    des4. es8~ es2~  | es2. r8 gis~ | gis1~ | gis4t a gis fis gis fis |\n    e4. fis8~ fis2~  | fis2. r8 a~ | a1~ | a4t bes a g a g |\n    f4. g8~g2~ | g2 r4t a bes | c' c d e f g | gis2. a4 | \n    bes4t bes, c d e f | fis1 | g4. e8 e d4 g8~ | g4 e8 e4 e8 d g~ |\n    g4 e e d8 g~ | g8 g e e4 e8 d a~ | a4. f8 f f d c' | c'4. e8 e4t e d |\n    e1~ | e4 r2. |\n}  \n]\n\n\n[\ntype: sheet;\n{\n    /style: bossanova normal/\n    Fmaj7 | Fmaj7 | G7 | G7 |\n    G-7 | Ges7 | Fmaj7 | Ges7 |\n    Fmaj7 | Fmaj7 | G7 | G7 |\n    G-7 | Ges7 | \n    Fmaj7 | Fmaj7 | Gesmaj7 | Gesmaj7 |\n    B7 | B7 | Fis-7 | Fis-7 |\n    D7 | D7 | G-7 | G-7 |\n    Es7 | Es7 | A-7 | D7b9 |\n    G-7 | C7b9 | Fmaj7 | Fmaj7 |\n    G7 | G7 | G-7 | Ges7 |\n    Fmaj7 | Ges9 |\n}\n]";

	auto rc = sheet::getRowAndColumn(source.begin(), source.end(), 650);
	BOOST_CHECK(std::get<0>(rc) == 26);
	BOOST_CHECK(std::get<1>(rc) == 4);
	rc = sheet::getRowAndColumn(source.begin(), source.end(), 1363);
	BOOST_CHECK(std::get<0>(rc) == 45);
	BOOST_CHECK(std::get<1>(rc) == 4);
}

BOOST_AUTO_TEST_CASE(test_map_arguments_by_keyword)
{
	std::vector<fm::String> args = {
			"keyword1", "value1", "value2",
			"keyword2", "value3", "value4", "value5",
	};
	std::vector<fm::String> keywords = {"keyword1", "keyword2"};
	auto keywordsAndValues = sheet::mapArgumentsByKeywords(args, keywords);
	BOOST_CHECK(keywordsAndValues.size() == 2);
	BOOST_CHECK(keywordsAndValues.count("keyword1") == 1);
	BOOST_CHECK(keywordsAndValues.count("keyword2") == 1);
	auto argContainerIt = keywordsAndValues.equal_range("keyword1").first;
	auto it = argContainerIt->second.begin();
	BOOST_CHECK(*(it++) == "value1");
	BOOST_CHECK(*it == "value2");
	argContainerIt = keywordsAndValues.equal_range("keyword2").first;
	it = argContainerIt->second.begin();
	BOOST_CHECK(*(it++) == "value3");
	BOOST_CHECK(*(it++) == "value4");	
	BOOST_CHECK(*(it) == "value5");	
}

BOOST_AUTO_TEST_CASE(test_map_arguments_by_keyword_2)
{
	std::vector<fm::String> args = {"value1", "value2", "value3"};
	std::vector<fm::String> keywords = {"keyword1", "keyword2"};
	auto keywordsAndValues = sheet::mapArgumentsByKeywords(args, keywords);
	BOOST_CHECK(keywordsAndValues.size() == 1);
	BOOST_CHECK(keywordsAndValues.count("") == 1);
	auto argsIt = keywordsAndValues.equal_range("").first;
	auto it = argsIt->second.begin();
	BOOST_CHECK(*(it++) == "value1");
	BOOST_CHECK(*(it++) == "value2");	
	BOOST_CHECK(*(it) == "value3");	
}

BOOST_AUTO_TEST_CASE(test_map_arguments_by_keyword_3)
{
	std::vector<fm::String> args = {
		"keyword1", "value1", "value2", 
		"keyword1", "value3"
	};
	std::vector<fm::String> keywords = {"keyword1", "keyword2"};
	auto keywordsAndValues = sheet::mapArgumentsByKeywords(args, keywords);
	BOOST_CHECK(keywordsAndValues.size() == 2);
	BOOST_CHECK(keywordsAndValues.count("keyword1") == 2);
	auto argsIt = keywordsAndValues.equal_range("keyword1").first;
	auto it = argsIt->second.begin();
	BOOST_CHECK(*(it++) == "value1");
	BOOST_CHECK(*(it++) == "value2");
	++argsIt;
	it = argsIt->second.begin();
	BOOST_CHECK(*(it) == "value3");	
}

#if 0
BOOST_AUTO_TEST_CASE(test_sconv)
{
	std::wstring wstr = fm::to_wstring("äöüÄÖÜ?§");
	BOOST_CHECK(L"äöüÄÖÜ?§" == wstr);

	std::string str = L"äöüÄÖÜ?§";
	BOOST_CHECK("äöüÄÖÜ?§" == str);
}
#endif


