#ifndef PARSER_H
#define PARSER_H
#include "IDocumentParser.h"
#include <fm/common.hpp>
#include <vector>
#include <sheet/objects/ChordDef.h>
#include <sheet/objects/ConductionSheetDef.h>
#include "sheet/SheetDef.h"
#include "sheet/Pitchmap.h"
#include "sheet/DocumentUsing.h"
#include "forward.hpp"
#include <tuple>

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

		struct SheetDefParser {

			SheetDef parse(fm::CharType const* first, fm::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			SheetDef parse(const fm::String &str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
		};


		struct ConfigParser {
			SheetDef parse(fm::CharType const* first, fm::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			SheetDef parse(const fm::String& str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
		};

		struct ConductionSheetParser {
			ConductionSheetDef parse(fm::CharType const* first, fm::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			ConductionSheetDef parse(const fm::String& str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				fm::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
			virtual ~ConductionSheetParser() = default;
		};

		class DocumentParser : public IDocumentParser {
		public:
			DocumentParser(DocumentPtr document) : _document(document) {} 
			typedef std::vector<fm::String> Usings;
			virtual DocumentPtr parse(const fm::String &path);
			virtual DocumentPtr parseString(const fm::String &text);
			virtual ~DocumentParser() = default;
		private:
			DocumentPtr _document;
		};
	}
}


#endif