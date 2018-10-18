#include <boost/test/unit_test.hpp>
#include <fm/common.hpp>
#include <fm/midi.hpp>

BOOST_AUTO_TEST_CASE(test_endswap)
{
	using namespace fm;
	if (isLittleEndian()) {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_TEST(bytes[0] == 0xEF);
		BOOST_TEST(bytes[1] == 0xCD);
		BOOST_TEST(bytes[2] == 0xAB);
		BOOST_TEST(bytes[3] == 0xAA);
		endswap(&x);
		BOOST_TEST(bytes[0] == 0xAA);
		BOOST_TEST(bytes[1] == 0xAB);
		BOOST_TEST(bytes[2] == 0xCD);
		BOOST_TEST(bytes[3] == 0xEF);
	}
	else {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_TEST(bytes[0] == 0xAA);
		BOOST_TEST(bytes[1] == 0xAB);
		BOOST_TEST(bytes[2] == 0xCD);
		BOOST_TEST(bytes[3] == 0xEF);
		endswap(&x);
		BOOST_TEST(bytes[0] == 0xEF);
		BOOST_TEST(bytes[1] == 0xCD);
		BOOST_TEST(bytes[2] == 0xAB);
		BOOST_TEST(bytes[3] == 0xAA);
	}
}