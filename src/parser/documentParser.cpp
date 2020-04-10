#include "parser.h"
#include "sheet/Document.h"
#include <boost/filesystem.hpp>
#include "fm/config.hpp"
#include <unordered_map>
#include <functional>
#include "fm/common.hpp"
#include "fm/werckmeister.hpp"
#include "error.hpp"
#include <sheet/tools.h>
#include <set>


namespace sheet {
	namespace compiler {

		namespace {
			typedef std::function<void(DocumentPtr, const fm::String&, Event::SourceId)> ExtHandler;
			typedef std::set<std::string> Extensions;
			void useChordDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void usePitchmapDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useLuaScript(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void useSheetTemplateDef(DocumentPtr doc, const fm::String &path, Event::SourceId);
			void processUsings(DocumentPtr doc, 
				const sheet::DocumentUsing &documentUsing, 
				const Extensions &allowedExtendions,
				const fm::String &sourcePath = fm::String());
			
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ SHEET_TEMPLATE_DEF_EXTENSION , &useSheetTemplateDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef },
				{ LUA_DEF_EXTENSION , &useLuaScript }
			});
			
			const Extensions AllSupportedExtensions = {
				CHORD_DEF_EXTENSION,
				SHEET_TEMPLATE_DEF_EXTENSION,
				PITCHMAP_DEF_EXTENSION,
				LUA_DEF_EXTENSION
			};

			void append(DocumentPtr doc, const SheetDef &sheetDef)
			{
				append(doc->sheetDef.documentConfigs, sheetDef.documentConfigs);
				append(doc->sheetDef.tracks, sheetDef.tracks);
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

			void processUsings(DocumentPtr doc, 
				const sheet::DocumentUsing &documentUsing,
				const Extensions &allowedExtendions,
				const fm::String &sourcePath)
			{
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
					absolutePath = fm::getWerckmeister().resolvePath(x, doc, sourcePath);
					auto sourceId = doc->addSource(absolutePath);
					it->second(doc, absolutePath, sourceId);
				}
			}						
		}

		DocumentPtr DocumentParser::parse(const fm::String &path, DocumentPtr input)
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

			auto res = !!input ? input : fm::getWerckmeister().createDocument();
			res->path = wm.absolutePath(path);
			auto sourceId = res->addSource(res->path);
			res->sourceId = sourceId;
			SheetDefParser sheetParser;
			try {
				res->sheetDef = sheetParser.parse(first, last, sourceId);
				processUsings(res, res->sheetDef.documentUsing, AllSupportedExtensions);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_document(res);
				throw;
			}
			return res;
		}

		DocumentPtr DocumentParser::parseString(const fm::String &documentText)
		{
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			auto res = fm::getWerckmeister().createDocument();
			res->sourceId = 0;
			SheetDefParser sheetParser;
			try {
				res->sheetDef = sheetParser.parse(first, last, 0);
				//processUsings(res, res->sheetDef.documentUsing, AllSupportedExtensions);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_document(res);
				throw;
			}
			return res;
		}
	}
}