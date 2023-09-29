#pragma once
#include "IDocumentParser.h"
#include <com/common.hpp>
#include <vector>
#include <documentModel/objects/ChordDef.h>
#include <documentModel/objects/ConductionSheetDef.h>
#include "documentModel/SheetDef.h"
#include "documentModel/Pitchmap.h"
#include "forward.hpp"
#include <tuple>

namespace parser
{
	struct AParser
	{
		virtual ~AParser() = default;
	};
	struct ChordDefParser
	{
		static const char *ALLOWED_CHORD_SYMBOLS_REGEX;
		typedef std::vector<documentModel::ChordDef> ChordDefs;
		ChordDefs parse(com::CharType const *first, com::CharType const *last);
		ChordDefs parse(const com::String &str)
		{
			com::CharType const *cstr = str.c_str();
			return parse(cstr, cstr + str.length());
		}
	};
	struct PitchmapParser
	{
		typedef std::vector<documentModel::Pitchmap> PitchmapDefs;
		PitchmapDefs parse(com::CharType const *first, com::CharType const *last);
		PitchmapDefs parse(const com::String &str)
		{
			com::CharType const *cstr = str.c_str();
			return parse(cstr, cstr + str.length());
		}
	};

	struct SheetDefParser
	{

		documentModel::SheetDef parse(com::CharType const *first, com::CharType const *last, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource);
		documentModel::SheetDef parse(const com::String &str, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource)
		{
			com::CharType const *cstr = str.c_str();
			return parse(cstr, cstr + str.length(), sourceId);
		}
	};

	struct ConfigParser
	{
		documentModel::SheetDef parse(com::CharType const *first, com::CharType const *last, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource);
		documentModel::SheetDef parse(const com::String &str, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource)
		{
			com::CharType const *cstr = str.c_str();
			return parse(cstr, cstr + str.length(), sourceId);
		}
	};

	struct ConductionSheetParser
	{
		documentModel::ConductionSheetDef parse(com::CharType const *first, com::CharType const *last, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource);
		documentModel::ConductionSheetDef parse(const com::String &str, documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource)
		{
			com::CharType const *cstr = str.c_str();
			return parse(cstr, cstr + str.length(), sourceId);
		}
		virtual ~ConductionSheetParser() = default;
	};

	class DocumentParser : public IDocumentParser
	{
	public:
		DocumentParser(documentModel::DocumentPtr document) : _document(document) {}
		typedef std::vector<com::String> Usings;
		virtual documentModel::DocumentPtr parse(const com::String &path);
		virtual documentModel::DocumentPtr parseString(const com::String &text);
		virtual ~DocumentParser() = default;

	private:
		documentModel::DocumentPtr _document;
	};
}
