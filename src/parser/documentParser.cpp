#include "parser.h"
#include "sheet/Document.h"
#include <boost/filesystem.hpp>
#include "fm/config.hpp"
#include <unordered_map>
#include <functional>
#include "fm/common.hpp"
#include "fm/werckmeister.hpp"
#include "error.hpp"
#include <fm/tools.h>
#include <set>


namespace sheet {
	namespace compiler {

		namespace {
			typedef std::function<void(DocumentPtr, const fm::String&, Event::SourceId)> ExtHandler;
			typedef std::set<std::string> Extensions;
			void useChordDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void usePitchmapDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useStyleSheet(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useLuaScript(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useLuaScript(DocumentPtr doc, const fm::String& path, Event::SourceId);
			void useSheetTemplateDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useConfig(DocumentPtr doc, const fm::String& path, Event::SourceId sourceId);
			void processUsings(DocumentPtr doc, 
				const sheet::DocumentUsing &documentUsing, 
				const Extensions &allowedExtendions,
				const fm::String &sourcePath = fm::String());
			
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ SHEET_TEMPLATE_DEF_EXTENSION , &useSheetTemplateDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef },
				{ LUA_DEF_EXTENSION , &useLuaScript },
				{ SHEET_CONFIG , &useConfig },
				{ CONDUCTIONS_SHEET , &useStyleSheet }
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
				fm::append(doc->sheetDef.documentConfigs, sheetDef.documentConfigs);
				fm::append(doc->sheetDef.tracks, sheetDef.tracks);
			}

			void useChordDef(DocumentPtr doc, const fm::String &path, Event::SourceId sourceId)
			{
				auto filestream = fm::getWerckmeister().openResource(path);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				ChordDefParser chordDefParser;
				auto chords = chordDefParser.parse(documentText);
				for (const auto &x : chords) {
					doc->chordDefs[x.name] = x;
				}
			}
			void usePitchmapDef(DocumentPtr doc, const fm::String &path, Event::SourceId sourceId)
			{
				
				auto filestream = fm::getWerckmeister().openResource(path);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				PitchmapParser pitchmapParser;
				auto pitchmaps = pitchmapParser.parse(documentText);
				for (const auto &x : pitchmaps) {
					doc->pitchmapDefs[x.name] = x.pitch;
				}
			}
			void useStyleSheet(DocumentPtr doc, const fm::String &path, Event::SourceId sourceId)
			{
				
				auto filestream = fm::getWerckmeister().openResource(path);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				ConductionSheetParser parser;
				auto conductionSheets = parser.parse(documentText);
				doc->conductionSheets = conductionSheets;
			}
			void useLuaScript(DocumentPtr doc, const fm::String &path, Event::SourceId sourceId)
			{
				auto &wm = fm::getWerckmeister();
				wm.registerLuaScript(path);
			}

			void useSheetTemplateDef(DocumentPtr doc, const fm::String &path, Event::SourceId sourceId)
			{
				try {
					
					auto filestream = fm::getWerckmeister().openResource(path);
					fm::StreamBuffIterator begin(*filestream);
					fm::StreamBuffIterator end;
					fm::String documentText(begin, end);
					SheetDefParser sheetDefParser;
					auto sheetDef = sheetDefParser.parse(documentText, sourceId);
					append(doc, sheetDef);
					processUsings(doc, sheetDef.documentUsing, {LUA_DEF_EXTENSION, PITCHMAP_DEF_EXTENSION}, path);
				} catch (Exception &ex) {
					ex << ex_error_source_file(path);
					throw;
				}
			}

			void useConfig(DocumentPtr doc, const fm::String& path, Event::SourceId sourceId)
			{
				try {
					auto filestream = fm::getWerckmeister().openResource(path);
					fm::StreamBuffIterator begin(*filestream);
					fm::StreamBuffIterator end;
					fm::String documentText(begin, end);
					ConfigParser configParser;
					auto configDef = configParser.parse(documentText, sourceId);
					fm::append(doc->sheetDef.documentConfigs, configDef.documentConfigs);
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
				const fm::String &sourcePath)
			{
				auto& wm = fm::getWerckmeister();
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
					fm::String absolutePath;
					wm.addSearchPath(sourcePath);
					absolutePath = wm.resolvePath(x);
					auto sourceId = doc->addSource(absolutePath);
					it->second(doc, absolutePath, sourceId);
				}
			}						
		}

		DocumentPtr DocumentParser::parse(const fm::String &path)
		{
			auto &wm = fm::getWerckmeister();
			auto filestream = wm.openResource(path);
			auto directory = boost::filesystem::path(path).parent_path().generic_string();
			wm.addSearchPath(directory);
			fm::StreamBuffIterator begin(*filestream);
			fm::StreamBuffIterator end;
			fm::String documentText(begin, end);
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			_document->path = wm.absolutePath(path);
			auto sourceId = _document->addSource(_document->path);
			_document->sourceId = sourceId;
			SheetDefParser sheetParser;
			try {
				_document->sheetDef = sheetParser.parse(first, last, sourceId);
				processUsings(_document, _document->sheetDef.documentUsing, AllSupportedExtensions);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_document(_document);
				throw;
			}
			wm.addSearchPath(_document->path);
			return _document;
		}

		DocumentPtr DocumentParser::parseString(const fm::String &documentText)
		{
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			_document->sourceId = 0;
			SheetDefParser sheetParser;
			try {
				_document->sheetDef = sheetParser.parse(first, last, 0);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_document(_document);
				throw;
			}
			return _document;
		}
	}
}