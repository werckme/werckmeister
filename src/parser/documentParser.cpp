#include "parser.h"
#include "documentModel/Document.h"
#include <boost/filesystem.hpp>
#include "com/config.hpp"
#include <unordered_map>
#include <functional>
#include "com/common.hpp"
#include "com/werckmeister.hpp"
#include "error.hpp"
#include <com/tools.h>
#include <set>

namespace parser
{

	namespace
	{
		using namespace documentModel;
		typedef std::function<void(DocumentPtr, const com::String &, Event::SourceId)> ExtHandler;
		typedef std::set<std::string> Extensions;
		void useChordDef(DocumentPtr doc, const com::String &path, Event::SourceId);
		void usePitchmapDef(DocumentPtr doc, const com::String &path, Event::SourceId);
		void useConductionSheet(DocumentPtr doc, const com::String &path, Event::SourceId);
		void useLuaScript(DocumentPtr doc, const com::String &path, Event::SourceId);
		void useLuaScript(DocumentPtr doc, const com::String &path, Event::SourceId);
		void useSheetTemplateDef(DocumentPtr doc, const com::String &path, Event::SourceId);
		void useConfig(DocumentPtr doc, const com::String &path, Event::SourceId sourceId);
		void useSheetPart(DocumentPtr doc, const com::String &path, Event::SourceId sourceId);
		void processUsings(DocumentPtr doc,
						   const documentModel::SheetDef::Usings &documentUsings,
						   const Extensions &allowedExtendions,
						   const com::String &sourcePath = com::String());

		std::unordered_map<std::string, ExtHandler> exthandlers({{CHORD_DEF_EXTENSION, &useChordDef},
																 {SHEET_TEMPLATE_DEF_EXTENSION, &useSheetTemplateDef},
																 {PITCHMAP_DEF_EXTENSION, &usePitchmapDef},
																 {LUA_DEF_EXTENSION, &useLuaScript},
																 {SHEET_CONFIG, &useConfig},
																 {SHEET_PART, &useSheetPart},
																 {CONDUCTIONS_SHEET, &useConductionSheet}});

		const Extensions AllSupportedExtensions = {
			CHORD_DEF_EXTENSION,
			SHEET_TEMPLATE_DEF_EXTENSION,
			PITCHMAP_DEF_EXTENSION,
			LUA_DEF_EXTENSION,
			SHEET_CONFIG,
			CONDUCTIONS_SHEET,
			SHEET_PART};

		const Extensions AllSupportedPartExtensions = {
			CHORD_DEF_EXTENSION,
			SHEET_TEMPLATE_DEF_EXTENSION,
			PITCHMAP_DEF_EXTENSION,
			LUA_DEF_EXTENSION,
			SHEET_CONFIG,
			CONDUCTIONS_SHEET};

		void append(DocumentPtr doc, const SheetDef &sheetDef)
		{
			com::append(doc->sheetDef.documentConfigs, sheetDef.documentConfigs);
			com::append(doc->sheetDef.tracks, sheetDef.tracks);
		}

		void merge(DocumentPtr target, DocumentPtr src)
		{
			append(target, src->sheetDef);
			com::append(src->conductionSheets, target->conductionSheets);
			com::insertRange(src->chordDefs, target->chordDefs);
			com::insertRange(src->pitchmapDefs, target->pitchmapDefs);
			com::insertRange(src->sources, target->sources);
		}

		void useChordDef(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{
			auto filestream = com::getWerckmeister().openResource(path);
			com::StreamBuffIterator begin(*filestream);
			com::StreamBuffIterator end;
			com::String documentText(begin, end);
			ChordDefParser chordDefParser;
			auto chords = chordDefParser.parse(documentText);
			for (const auto &x : chords)
			{
				doc->chordDefs[x.name] = x;
			}
		}
		void usePitchmapDef(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{

			auto filestream = com::getWerckmeister().openResource(path);
			com::StreamBuffIterator begin(*filestream);
			com::StreamBuffIterator end;
			com::String documentText(begin, end);
			PitchmapParser pitchmapParser;
			auto pitchmaps = pitchmapParser.parse(documentText);
			for (const auto &x : pitchmaps)
			{
				doc->pitchmapDefs[x.name] = x.pitch;
			}
		}
		void useConductionSheet(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{

			auto filestream = com::getWerckmeister().openResource(path);
			com::StreamBuffIterator begin(*filestream);
			com::StreamBuffIterator end;
			com::String documentText(begin, end);
			ConductionSheetParser parser;
			auto conductionSheet = parser.parse(documentText, sourceId);
			doc->conductionSheets.push_back(conductionSheet);
		}
		void useLuaScript(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{
			auto &wm = com::getWerckmeister();
			wm.registerLuaScript(path);
		}

		void useSheetTemplateDef(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{
			try
			{

				auto filestream = com::getWerckmeister().openResource(path);
				com::StreamBuffIterator begin(*filestream);
				com::StreamBuffIterator end;
				com::String documentText(begin, end);
				SheetDefParser sheetDefParser;
				auto sheetDef = sheetDefParser.parse(documentText, sourceId);
				append(doc, sheetDef);
				processUsings(doc, sheetDef.documentUsings, {LUA_DEF_EXTENSION, PITCHMAP_DEF_EXTENSION, CONDUCTIONS_SHEET}, path);
			}
			catch (compiler::Exception &ex)
			{
				ex << compiler::ex_error_source_file(path);
				throw;
			}
		}
		void useConfig(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
		{
			try
			{
				auto filestream = com::getWerckmeister().openResource(path);
				com::StreamBuffIterator begin(*filestream);
				com::StreamBuffIterator end;
				com::String documentText(begin, end);
				ConfigParser configParser;
				auto configDef = configParser.parse(documentText, sourceId);
				com::append(doc->sheetDef.documentConfigs, configDef.documentConfigs);
				processUsings(doc, configDef.documentUsings, 
				{
					LUA_DEF_EXTENSION, 
					PITCHMAP_DEF_EXTENSION, 
					CHORD_DEF_EXTENSION, 
					SHEET_TEMPLATE_DEF_EXTENSION,
					CONDUCTIONS_SHEET
				}, 
				path);
			}
			catch (compiler::Exception &ex)
			{
				ex << compiler::ex_error_source_file(path);
				throw;
			}
		}

		void useSheetPart(DocumentPtr mainDocument, const com::String &path, Event::SourceId sourceId)
		{
			auto documentPtr = std::make_shared<documentModel::Document>();
			documentPtr->path = path;
			DocumentParser parser(documentPtr);
			parser.isPart = true;
			parser.parse(path);
			merge(mainDocument, documentPtr);
		}

		void processUsings(DocumentPtr doc,
						   const documentModel::SheetDef::Usings &documentUsings,
						   const Extensions &allowedExtendions,
						   const com::String &sourcePath)
		{
			try 
			{
				auto &wm = com::getWerckmeister();
				for (const auto &x : documentUsing.usings)
				{
					auto path = boost::filesystem::path(x);
					auto ext = path.extension().string();
					auto it = exthandlers.find(ext);
					if (it == exthandlers.end())
					{
						FM_THROW(compiler::Exception, "unsupported file type: " + x);
					}
					if (allowedExtendions.find(ext) == allowedExtendions.end())
					{
						FM_THROW(compiler::Exception, "document type not allowed: " + x);
					}
					com::String absolutePath;
					wm.addSearchPath(sourcePath);
					absolutePath = wm.resolvePath(x);
					auto sourceId = doc->addSource(absolutePath);
					it->second(doc, absolutePath, sourceId);
				}
			} 
			catch(compiler::Exception &ex)
			{
				ASheetObjectWithSourceInfo posInfo;
				posInfo.sourceId = doc->sourceId;
				posInfo.sourcePositionBegin = 0;
				posInfo.sourcePositionEnd = 0;
				ex << compiler::ex_sheet_source_info(posInfo);
				throw;
			}
		}
	}

	DocumentPtr DocumentParser::parse(const com::String &path)
	{
		auto &wm = com::getWerckmeister();
		auto filestream = wm.openResource(path);
		auto directory = boost::filesystem::path(path).parent_path().generic_string();
		wm.addSearchPath(directory);
		com::StreamBuffIterator begin(*filestream);
		com::StreamBuffIterator end;
		com::String documentText(begin, end);
		const com::String::value_type *first = documentText.c_str();
		const com::String::value_type *last = first + documentText.length();

		_document->path = wm.absolutePath(path);
		auto sourceId = _document->addSource(_document->path);
		_document->sourceId = sourceId;
		SheetDefParser sheetParser;
		try
		{
			_document->sheetDef = sheetParser.parse(first, last, sourceId);
			processUsings(_document, 
				_document->sheetDef.documentUsing, 
				this->isPart ? AllSupportedPartExtensions : AllSupportedExtensions);
		}
		catch (com::Exception &ex)
		{
			ex << compiler::ex_sheet_document(_document);
			throw;
		}
		wm.addSearchPath(_document->path);
		return _document;
	}

	DocumentPtr DocumentParser::parseString(const com::String &documentText)
	{
		const com::String::value_type *first = documentText.c_str();
		const com::String::value_type *last = first + documentText.length();

		_document->sourceId = 0;
		SheetDefParser sheetParser;
		try
		{
			_document->sheetDef = sheetParser.parse(first, last, 0);
		}
		catch (com::Exception &ex)
		{
			ex << compiler::ex_sheet_document(_document);
			throw;
		}
		return _document;
	}
}