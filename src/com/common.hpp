#pragma once

#define FM_STRING(str) str
#define FM_CHAR(str) str
#define FM_COUT std::cout

#include <string>
#include <sstream>
#include "units.hpp"
#include <map>
#include <algorithm>
#include <cstdlib>

#define FM_THROW(ex, msg) throw ex((msg), __FILE__, __LINE__)

#define DEPRECATED

namespace com
{
	typedef unsigned char Byte;
	typedef char CharType;
	typedef std::string UId;
	typedef std::string String;
	typedef String Path;
	typedef std::stringstream StringStream;
	typedef std::istreambuf_iterator<String::value_type> StreamBuffIterator;
	bool isLittleEndian();

	template <typename TType>
	TType absDifference(const TType &a, const TType &b)
	{
		// we dont use abs, because in case of unsigned int smaller - bigger = insanely big
		auto max = std::max(a, b);
		auto min = std::min(a, b);
		return (max - min);
	}

	template <typename TType>
	bool compareTolerant(const TType &a, const TType &b, const TType &range)
	{
		return absDifference(a, b) <= range;
	}

	template <typename TType>
	void endswap(TType *objp)
	{
		// https://stackoverflow.com/questions/3823921/convert-big-endian-to-little-endian-when-reading-from-a-binary-file
		unsigned char *memp = reinterpret_cast<unsigned char *>(objp);
		std::reverse(memp, memp + sizeof(TType));
	}
	UId generateUid();

}
