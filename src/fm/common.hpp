#ifndef FM_COMMON_HPP
#define FM_COMMON_HPP

#define FM_STRING(str) L##str
#define FM_COUT std::wcout

#include <string>
#include <sstream>
#include "units.hpp"
#include <map>
#include <algorithm>
#include <stdlib.h>

namespace fm {
	typedef unsigned char Byte;
	typedef wchar_t CharType;
	typedef std::string UId;
    typedef std::wstring String;
    typedef std::wstringstream StringStream;
	typedef std::istreambuf_iterator<String::value_type> StreamBuffIterator;
	bool isLittleEndian();

	template<typename TType>
	void endswap(TType *objp)
	{
		// https://stackoverflow.com/questions/3823921/convert-big-endian-to-little-endian-when-reading-from-a-binary-file
		unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
		std::reverse(memp, memp + sizeof(TType));
	}
	UId generateUid();

	inline std::string to_string(const fm::String &str)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//return converter.from_bytes(str); // this failed for some reason on windows with path names and umlaute
		std::stringstream ss;
		char mb[MB_LEN_MAX];
		for (auto ch : str) {
			int length = 0;
			wctomb_s(&length, &mb[0], MB_LEN_MAX, ch);
			ss.write(&mb[0], length);
		}
		return ss.str();
	}

	inline std::wstring to_wstring(const std::string &str)
	{
		std::wstringstream ss;
		for (auto ch : str) {
			ss << ch;
		}
		return ss.str();
	}

}

#endif