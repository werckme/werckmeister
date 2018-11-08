#include "common.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/locale.hpp>
#include "config.hpp"


namespace {
	union {
		unsigned short shortVar;    // binaere Ganzzahl der Laenge 16 Bits
		unsigned char  charVar[2];  // 2 binaere Ganzzahlen, jede 8 Bits
	} test_endianness;
}

namespace fm {
	// https://de.wikipedia.org/wiki/Byte-Reihenfolge
	bool isLittleEndian() {
		test_endianness.shortVar = 0x8000; // das Most Significant Bit innerhalb von 16
		if (test_endianness.charVar[0] != 0) {
			// Das Programm laeuft auf einer Big-Endian-Maschine.
			return false;
		}
		else {
			// Das Programm laeuft auf einer Little-Endian-Maschine.
			return true;
		}
	}

	UId generateUid()
	{
        boost::uuids::uuid u = boost::uuids::random_generator()();
        return boost::uuids::to_string(u);
	}

	std::string to_string(const fm::String &str)
	{
		return boost::locale::conv::from_utf(str, FM_CHARSET);
	}

	std::wstring to_wstring(const std::string &str)
	{
		return boost::locale::conv::to_utf<wchar_t>(str, FM_CHARSET);
	}
}