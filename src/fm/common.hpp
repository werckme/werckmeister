#ifndef FM_COMMON_HPP
#define FM_COMMON_HPP

#define FM_STRING(str) L##str
#define FM_COUT std::wcout

#include <string>
#include <sstream>
#include "units.hpp"
#include <map>
#include <algorithm>

namespace fm {
	typedef unsigned char Byte;
	typedef wchar_t CharType;
	typedef std::string UId;
    typedef std::wstring String;
    typedef std::wstringstream StringStream;
	bool isLittleEndian();

	template<typename TType>
	void endswap(TType *objp)
	{
		// https://stackoverflow.com/questions/3823921/convert-big-endian-to-little-endian-when-reading-from-a-binary-file
		unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
		std::reverse(memp, memp + sizeof(TType));
	}
	UId generateUid();

}

#endif