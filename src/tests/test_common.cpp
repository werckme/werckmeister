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


BOOST_AUTO_TEST_CASE(position_to_row_and_column)
{
		fm::String source = FM_STRING("12345678901234567890\n\
12345678901234567890\n\
12345\n\
12\n\
1");

	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 0) == sheet::RowAndColumn(0, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 4) == sheet::RowAndColumn(0, 4));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 19) == sheet::RowAndColumn(0, 19));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 20) == sheet::RowAndColumn(1, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 39) == sheet::RowAndColumn(1, 19));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 40) == sheet::RowAndColumn(2, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 44) == sheet::RowAndColumn(2, 4));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 45) == sheet::RowAndColumn(3, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 46) == sheet::RowAndColumn(3, 1));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 47) == sheet::RowAndColumn(4, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 48) == sheet::InvalidRowAndColumn);
}

BOOST_AUTO_TEST_CASE(position_to_row_and_column_2)
{
		std::string source = "12345678901234567890\n\
12345678901234567890\n\
12345\n\
12\n\
1";

	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 0) == sheet::RowAndColumn(0, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 4) == sheet::RowAndColumn(0, 4));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 19) == sheet::RowAndColumn(0, 19));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 20) == sheet::RowAndColumn(1, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 39) == sheet::RowAndColumn(1, 19));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 40) == sheet::RowAndColumn(2, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 44) == sheet::RowAndColumn(2, 4));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 45) == sheet::RowAndColumn(3, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 46) == sheet::RowAndColumn(3, 1));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 47) == sheet::RowAndColumn(4, 0));
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 48) == sheet::InvalidRowAndColumn);
}

BOOST_AUTO_TEST_CASE(position_to_row_and_column_3)
{
	std::string source;
	BOOST_CHECK(sheet::getRowAndColumn(source.begin(), source.end(), 0) == sheet::InvalidRowAndColumn);
}


BOOST_AUTO_TEST_CASE(positions_to_rows_and_columns)
{
		fm::String source = FM_STRING("12345678901234567890\n\
12345678901234567890\n\
12345\n\
12\n\
1");

	std::vector<int> positions = { 48, 0, 47, 4, 46, 19, 45, 20, 44, 39, 40 };
	auto result = sheet::getRowsAndColumns(source.begin(), source.end(), positions);
	BOOST_CHECK(result [0] == sheet::RowAndColumn(0, 0));
	BOOST_CHECK(result [1] == sheet::RowAndColumn(0, 4));
	BOOST_CHECK(result [2] == sheet::RowAndColumn(0, 19));
	BOOST_CHECK(result [3] == sheet::RowAndColumn(1, 0));
	BOOST_CHECK(result [4] == sheet::RowAndColumn(1, 19));
	BOOST_CHECK(result [5] == sheet::RowAndColumn(2, 0));
	BOOST_CHECK(result [6] == sheet::RowAndColumn(2, 4));
	BOOST_CHECK(result [7] == sheet::RowAndColumn(3, 0));
	BOOST_CHECK(result [8] == sheet::RowAndColumn(3, 1));
	BOOST_CHECK(result [9] == sheet::RowAndColumn(4, 0));
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


