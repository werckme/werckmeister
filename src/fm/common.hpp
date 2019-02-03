#ifndef FM_COMMON_HPP
#define FM_COMMON_HPP

#define FM_STRING(str) L##str
#define FM_CHAR(str) L##str
#define FM_COUT std::wcout

#include <string>
#include <sstream>
#include "units.hpp"
#include <map>
#include <algorithm>
#include <cstdlib>

#define FM_THROW(ex, msg) throw ex((msg), __FILE__, __LINE__)

namespace fm {
	typedef unsigned char Byte;
	typedef wchar_t CharType;
	typedef std::string UId;
    typedef std::wstring String;
    typedef std::wstringstream StringStream;
	typedef std::istreambuf_iterator<String::value_type> StreamBuffIterator;
	bool isLittleEndian();

	template<typename TType>
	TType absDifference(const TType &a, const TType &b)
	{
		// we dont use abs, because in case of unsigned int smaller - bigger = insanely big
		auto max = std::max(a, b);
		auto min = std::min(a, b);
		return (max - min);
	}

	template<typename TType>
	bool compareTolerant(const TType &a, const TType &b, const TType &range)
	{
		return absDifference(a, b) <= range;
	}

	template<typename TType>
	void endswap(TType *objp)
	{
		// https://stackoverflow.com/questions/3823921/convert-big-endian-to-little-endian-when-reading-from-a-binary-file
		unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
		std::reverse(memp, memp + sizeof(TType));
	}
	UId generateUid();

	std::string to_string(const fm::String &str);

	std::wstring to_wstring(const std::string &str);

}

#endif