#ifndef PARSER_H
#define PARSER_H
#include <fm/common.hpp>
#include <vector>
#include "sheet/ChordDef.h"
#include "sheet/StyleDef.h"
#include "sheet/SheetDef.h"
#include "sheet/Pitchmap.h"
#include "sheet/DocumentConfig.h"
#include "forward.hpp"

namespace sheet {
	namespace compiler {
		struct AParser {
			virtual ~AParser() = default;
		};
		struct ChordDefParser {
			static const char* ALLOWED_CHORD_SYMBOLS_REGEX;
			typedef std::vector<ChordDef> ChordDefs;
			ChordDefs parse(fm::CharType const* first, fm::CharType const* last);
			ChordDefs parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
		struct PitchmapParser {
			typedef std::vector<Pitchmap> PitchmapDefs;
			PitchmapDefs parse(fm::CharType const* first, fm::CharType const* last);
			PitchmapDefs parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
		struct StyleDefParser {
			
			StyleDef parse(fm::CharType const* first, fm::CharType const* last);
			StyleDef parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
		struct SheetDefParser {

			SheetDef parse(fm::CharType const* first, fm::CharType const* last);
			SheetDef parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
		struct DocumentConfigParser {

			DocumentConfig parse(fm::CharType const* first, fm::CharType const* last);
			DocumentConfig parse(const fm::String &str)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};

		struct DocumentParser {

			DocumentPtr parse(const fm::String path);

		};
	}
}


#endif