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

namespace sheet {
	namespace compiler {

		namespace {

			void append(DocumentPtr doc, const SheetDef &sheetDef)
			{
				append(doc->sheetDef.sheetInfos, sheetDef.sheetInfos);
				append(doc->sheetDef.tracks, sheetDef.tracks);
			}

			fm::String getAbsolutePath(DocumentPtr doc, const fm::String &path)
			{
				return doc->getAbsolutePath(path);
			}
			void useChordDef(DocumentPtr doc, const fm::String &path)
			{
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
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
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				PitchmapParser pitchmapParser;
				auto pitchmaps = pitchmapParser.parse(documentText);
				for (const auto &x : pitchmaps) {
					doc->pitchmapDefs[x.name] = x.pitch;
				}
			}
			void useStyleDef(DocumentPtr doc, const fm::String &path)
			{
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				SheetDefParser sheetDefParser;
				auto name = boost::filesystem::path(path).stem().wstring();
				auto sheetDef = sheetDefParser.parse(documentText);
				append(doc, sheetDef);
			}
			typedef std::function<void(DocumentPtr, const fm::String&)> ExtHandler;
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ STYLE_DEF_EXTENSION , &useStyleDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef }
			});

			void processUsings(DocumentPtr doc)
			{
				for (const auto &x : doc->documentConfig.usings)
				{
					auto path = boost::filesystem::path(x);
					auto ext = path.extension().string();
					auto it = exthandlers.find(ext);
					if (it == exthandlers.end()) {
						FM_THROW(Exception, "unsupported file type: " + fm::to_string(x));
					}
					it->second(doc, x);
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

			{
				DocumentConfigParser configParser;
				res->documentConfig = configParser.parse(first, last);
			}
			{
				SheetDefParser sheetParser;
				res->sheetDef = sheetParser.parse(first, last);
			}

			processUsings(res);
			return res;
		}
	}
}