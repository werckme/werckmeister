#include <boost/test/unit_test.hpp>
#include <fm/common.hpp>
#include <fm/midi.hpp>
#include <fm/werckmeister.hpp>
#include <iterator>
#include <sheet/tools.h>

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
	auto resource = fm::getWerckmeister().openResource(FM_STRING("../sheets/chords/default.chords"));
	fm::StreamBuffIterator eos;
	fm::StreamBuffIterator it(*resource.get());
	fm::String res(it, eos);
	BOOST_CHECK(res.length() > 0);
}

// BOOST_AUTO_TEST_CASE(test_get_line_and_position_0)
// {
// 	fm::String line;
// 	int position = -1;
// 	std::tie(line, position) = sheet::getLineAndPosition(fm::String(FM_STRING("")), 0, false);
// 	BOOST_CHECK(line.length() == 0);
// 	BOOST_CHECK(position == -1);

// 	std::tie(line, position) = sheet::getLineAndPosition(fm::String(FM_STRING(" ")), 0, false);
// 	BOOST_CHECK(line.length() == 1);
// 	BOOST_CHECK(position == 0);

// 	std::tie(line, position) = sheet::getLineAndPosition(fm::String(FM_STRING(" ")), 0);
// 	BOOST_CHECK(line.length() == 0);
// 	BOOST_CHECK(position == -1);
// }

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

#if 0
BOOST_AUTO_TEST_CASE(test_sconv)
{
	std::wstring wstr = fm::to_wstring("äöüÄÖÜ?§");
	BOOST_CHECK(L"äöüÄÖÜ?§" == wstr);

	std::string str = fm::to_string(L"äöüÄÖÜ?§");
	BOOST_CHECK("äöüÄÖÜ?§" == str);
}
#endif
