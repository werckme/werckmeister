#ifndef PARSER_H
#define PARSER_H
#include <fm/common.hpp>
#include <vector>
#include <sheet/ChordDef.h>

namespace sheet {
	namespace compiler {
		struct AParser {
			virtual ~AParser() = default;
		};
		struct ChordDefParser {
			typedef std::vector<ChordDef> ChordDefs;
			ChordDefs parse(fm::CharType const* first, fm::CharType const* last);
			ChordDefs parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
	}
}


#endif