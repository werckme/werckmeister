#ifndef PARSER_H
#define PARSER_H
#include "IDocumentParser.h"
#include <com/common.hpp>
#include <vector>
#include <documentModel/objects/ChordDef.h>
#include <documentModel/objects/ConductionSheetDef.h>
#include "documentModel/SheetDef.h"
#include "documentModel/Pitchmap.h"
#include "documentModel/DocumentUsing.h"
#include "forward.hpp"
#include <tuple>

namespace documentModel {
	namespace compiler {
		struct AParser {
			virtual ~AParser() = default;
		};
		struct ChordDefParser {
			static const char* ALLOWED_CHORD_SYMBOLS_REGEX;
			typedef std::vector<ChordDef> ChordDefs;
			ChordDefs parse(com::CharType const* first, com::CharType const* last);
			ChordDefs parse(const com::String &str)
			{
				com::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};
		struct PitchmapParser {
			typedef std::vector<Pitchmap> PitchmapDefs;
			PitchmapDefs parse(com::CharType const* first, com::CharType const* last);
			PitchmapDefs parse(const com::String &str)
			{
				com::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length());
			}
		};

		struct SheetDefParser {

			SheetDef parse(com::CharType const* first, com::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			SheetDef parse(const com::String &str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				com::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
		};


		struct ConfigParser {
			SheetDef parse(com::CharType const* first, com::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			SheetDef parse(const com::String& str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				com::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
		};

		struct ConductionSheetParser {
			ConductionSheetDef parse(com::CharType const* first, com::CharType const* last, Event::SourceId sourceId = Event::UndefinedSource);
			ConductionSheetDef parse(const com::String& str, Event::SourceId sourceId = Event::UndefinedSource)
			{
				com::CharType const* cstr = str.c_str();
				return parse(cstr, cstr + str.length(), sourceId);
			}
			virtual ~ConductionSheetParser() = default;
		};

		class DocumentParser : public IDocumentParser {
		public:
			DocumentParser(DocumentPtr document) : _document(document) {} 
			typedef std::vector<com::String> Usings;
			virtual DocumentPtr parse(const com::String &path);
			virtual DocumentPtr parseString(const com::String &text);
			virtual ~DocumentParser() = default;
		private:
			DocumentPtr _document;
		};
	}
}


#endif