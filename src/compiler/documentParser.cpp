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
			typedef std::function<void(DocumentPtr, const fm::String&)> ExtHandler;
			typedef std::set<std::string> Extensions;
			void useChordDef(DocumentPtr doc, const fm::String &path);
			void usePitchmapDef(DocumentPtr doc, const fm::String &path);
			void useLuaScript(DocumentPtr doc, const fm::String &path);
			void useStyleDef(DocumentPtr doc, const fm::String &path);
			void processUsings(DocumentPtr doc, 
				const sheet::DocumentConfig &documentConfig, 
				const Extensions &allowedExtendions,
				const fm::String &sourcePath = fm::String());
			
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ STYLE_DEF_EXTENSION , &useStyleDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef },
				{ LUA_DEF_EXTENSION , &useLuaScript }
			});
			
			const Extensions AllSupportedExtensions = {
				CHORD_DEF_EXTENSION,
				STYLE_DEF_EXTENSION,
				PITCHMAP_DEF_EXTENSION,
				LUA_DEF_EXTENSION
			};

			void append(DocumentPtr doc, const SheetDef &sheetDef)
			{
				append(doc->sheetDef.sheetInfos, sheetDef.sheetInfos);
				append(doc->sheetDef.tracks, sheetDef.tracks);
			}

			fm::String getAbsolutePath(DocumentPtr doc, const fm::String &path)
			{
				return doc->getAbsolutePath(path);
			}

			fm::String getAbsolutePath(fm::String base, const fm::String &path)
			{
				auto a = boost::filesystem::path(base).parent_path();
				auto b = boost::filesystem::path(path);
				auto x = boost::filesystem::absolute(b, a);
				return boost::filesystem::system_complete(x).wstring();
			}

			void useChordDef(DocumentPtr doc, const fm::String &path)
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
			void usePitchmapDef(DocumentPtr doc, const fm::String &path)
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
			void useLuaScript(DocumentPtr doc, const fm::String &path)
			{
				auto &wm = fm::getWerckmeister();
				wm.registerLuaScript(path);
			}

			void useStyleDef(DocumentPtr doc, const fm::String &path)
			{
				try {
					auto sourceId = doc->addSource(path);
					auto filestream = fm::getWerckmeister().openResource(path);
					fm::StreamBuffIterator begin(*filestream);
					fm::StreamBuffIterator end;
					fm::String documentText(begin, end);
					SheetDefParser sheetDefParser;
					auto sheetDef = sheetDefParser.parse(documentText, sourceId);
					append(doc, sheetDef);
					processUsings(doc, sheetDef.documentConfig, {LUA_DEF_EXTENSION, PITCHMAP_DEF_EXTENSION}, path);
				} catch (Exception &ex) {
					ex << ex_error_source_file(path);
					throw;
				}
			}

			void processUsings(DocumentPtr doc, 
				const sheet::DocumentConfig &documentConfig,
				const Extensions &allowedExtendions,
				const fm::String &sourcePath)
			{
				for (const auto &x : documentConfig.usings)
				{
					auto path = boost::filesystem::path(x);
					auto ext = path.extension().string();
					auto it = exthandlers.find(ext);
					if (it == exthandlers.end()) {
						FM_THROW(Exception, "unsupported file type: " + fm::to_string(x));
					}
					if (allowedExtendions.find(ext) == allowedExtendions.end()) {
						FM_THROW(Exception, "document type not allowed: " + fm::to_string(x));
					}
					fm::String absolutePath;
					if (!sourcePath.empty()) {
						absolutePath = getAbsolutePath(sourcePath, x);
					} else {
						absolutePath = getAbsolutePath(doc, x);
					}
					it->second(doc, absolutePath);
				}
			}						
		}

		DocumentPtr DocumentParser::parse(const fm::String path)
		{
			auto filestream = fm::getWerckmeister().openResource(path);
			fm::StreamBuffIterator begin(*filestream);
			fm::StreamBuffIterator end;
			fm::String documentText(begin, end);
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			auto res = std::make_shared<Document>();
			res->path = boost::filesystem::system_complete(path).wstring();
			auto sourceId = res->addSource(res->path);
			res->sourceId = sourceId;
			SheetDefParser sheetParser;
			try {
				res->sheetDef = sheetParser.parse(first, last, sourceId);
				processUsings(res, res->sheetDef.documentConfig, AllSupportedExtensions);
			} catch(fm::Exception &ex) {
				ex << ex_sheet_document(res);
				throw;
			}
			return res;
		}
	}
}