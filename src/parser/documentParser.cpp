#include "parser.h"
#include "sheet/Document.h"
#include <boost/filesystem.hpp>
#include "com/config.hpp"
#include <unordered_map>
#include <functional>
#include "com/common.hpp"
#include "com/werckmeister.hpp"
#include "error.hpp"
#include <com/tools.h>
#include <set>


namespace sheet {
	namespace compiler {

		namespace {
			typedef std::function<void(DocumentPtr, const com::String&, Event::SourceId)> ExtHandler;
			typedef std::set<std::string> Extensions;
			void useChordDef(DocumentPtr doc, const com::String &path, Event::SourceId);
			void usePitchmapDef(DocumentPtr doc, const com::String &path, Event::SourceId);
			void useConductionSheet(DocumentPtr doc, const com::String &path, Event::SourceId);
			void useLuaScript(DocumentPtr doc, const com::String &path, Event::SourceId);
			void useLuaScript(DocumentPtr doc, const com::String& path, Event::SourceId);
			void useSheetTemplateDef(DocumentPtr doc, const com::String &path, Event::SourceId);
			void useConfig(DocumentPtr doc, const com::String& path, Event::SourceId sourceId);
			void processUsings(DocumentPtr doc, 
				const sheet::DocumentUsing &documentUsing, 
				const Extensions &allowedExtendions,
				const com::String &sourcePath = com::String());
			
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ SHEET_TEMPLATE_DEF_EXTENSION , &useSheetTemplateDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef },
				{ LUA_DEF_EXTENSION , &useLuaScript },
				{ SHEET_CONFIG , &useConfig },
				{ CONDUCTIONS_SHEET , &useConductionSheet }
			});
			
			const Extensions AllSupportedExtensions = {
				CHORD_DEF_EXTENSION,
				SHEET_TEMPLATE_DEF_EXTENSION,
				PITCHMAP_DEF_EXTENSION,
				LUA_DEF_EXTENSION,
				SHEET_CONFIG,
				CONDUCTIONS_SHEET
			};

			void append(DocumentPtr doc, const SheetDef &sheetDef)
			{
				com::append(doc->sheetDef.documentConfigs, sheetDef.documentConfigs);
				com::append(doc->sheetDef.tracks, sheetDef.tracks);
			}

			void useChordDef(DocumentPtr doc, const com::String &path, Event::SourceId sourceId)
			{
				auto filestream = com::getWerckmeister().openResource(path);
				com::StreamBuffIterator begin(*filestream);
				com::StreamBuffIterator end;
				com::String documentText(begin, end);
				ChordDefParser chordDefParser;
				auto chords = chordDefParser.parse(documentText);
				for (const auto &x : chords) {
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
				for (const auto &x : pitchmaps) {
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
				try {
					
					auto filestream = com::getWerckmeister().openResource(path);
					com::StreamBuffIterator begin(*filestream);
					com::StreamBuffIterator end;
					com::String documentText(begin, end);
					SheetDefParser sheetDefParser;
					auto sheetDef = sheetDefParser.parse(documentText, sourceId);
					append(doc, sheetDef);
					processUsings(doc, sheetDef.documentUsing, {LUA_DEF_EXTENSION, PITCHMAP_DEF_EXTENSION}, path);
				} catch (Exception &ex) {
					ex << ex_error_source_file(path);
					throw;
				}
			}

			void useConfig(DocumentPtr doc, const com::String& path, Event::SourceId sourceId)
			{
				try {
					auto filestream = com::getWerckmeister().openResource(path);
					com::StreamBuffIterator begin(*filestream);
					com::StreamBuffIterator end;
					com::String documentText(begin, end);
					ConfigParser configParser;
					auto configDef = configParser.parse(documentText, sourceId);
					com::append(doc->sheetDef.documentConfigs, configDef.documentConfigs);
					processUsings(doc, configDef.documentUsing, {
						LUA_DEF_EXTENSION, 
						PITCHMAP_DEF_EXTENSION, 
						CHORD_DEF_EXTENSION,
						SHEET_TEMPLATE_DEF_EXTENSION
					}, path);
				}
				catch (Exception& ex) {
					ex << ex_error_source_file(path);
					throw;
				}
			}

			void processUsings(DocumentPtr doc, 
				const sheet::DocumentUsing &documentUsing,
				const Extensions &allowedExtendions,
				const com::String &sourcePath)
			{
				auto& wm = com::getWerckmeister();
				for (const auto &x : documentUsing.usings)
				{
					auto path = boost::filesystem::path(x);
					auto ext = path.extension().string();
					auto it = exthandlers.find(ext);
					if (it == exthandlers.end()) {
						FM_THROW(Exception, "unsupported file type: " + x);
					}
					if (allowedExtendions.find(ext) == allowedExtendions.end()) {
						FM_THROW(Exception, "document type not allowed: " + x);
					}
					com::String absolutePath;
					wm.addSearchPath(sourcePath);
					absolutePath = wm.resolvePath(x);
					auto sourceId = doc->addSource(absolutePath);
					it->second(doc, absolutePath, sourceId);
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
			try {
				_document->sheetDef = sheetParser.parse(first, last, sourceId);
				processUsings(_document, _document->sheetDef.documentUsing, AllSupportedExtensions);
			} catch(com::Exception &ex) {
				ex << ex_sheet_document(_document);
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
			try {
				_document->sheetDef = sheetParser.parse(first, last, 0);
			} catch(com::Exception &ex) {
				ex << ex_sheet_document(_document);
				throw;
			}
			return _document;
		}
	}
}