#include "werckmeister.hpp"
#include "midi.hpp"
#include "config.hpp"
#include <type_traits>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <exception>
#include <fstream>
#include "compiler/Compiler.h"
#include <compiler/context/MidiContext.h>
#include <memory>
#include "compiler/voicings/VoicingStrategy.h"
#include "compiler/voicingStrategies.h"
#include "compiler/voicings/Lua.h"
#include "compiler/spielanweisung/Arpeggio.h"
#include "compiler/spielanweisung/Vorschlag.h"
#include "compiler/spielanweisung/spielanweisungen.h"
#include "compiler/modification/LuaMod.h"
#include <sheet/Document.h>

namespace fm {
    
	
	Werckmeister & getWerckmeister()
    {
        typedef Loki::SingletonHolder<Werckmeister> Holder;
		return Holder::Instance();
    }

    const char * Werckmeister::version() const
    {
        return SHEET_VERSION;
    }

	midi::MidiPtr Werckmeister::createMidi()
	{
		return std::make_shared<midi::Midi>(PPQ);
	}


	Werckmeister::ResourceStream Werckmeister::openResourceImpl(const Path &path)
	{
		if (path.empty()) {
			FM_THROW(Exception, "tried to load an empty path");
		}
		auto fpath = boost::filesystem::system_complete(path);
		auto absolute = fpath.string();
		if (!boost::filesystem::exists(path))
		{
			FM_THROW(Exception, "resource not found: " + fpath.string());
		}
		Werckmeister::ResourceStream result = std::make_unique<std::ifstream>(absolute.c_str(), std::ios_base::binary);
		return result;
	}

	void Werckmeister::saveResource(const Path &path, const fm::String &dataAsStr)
	{
		if (path.empty()) {
			FM_THROW(Exception, "tried to save an empty path");
		}
		auto fpath = boost::filesystem::system_complete(path);
		auto absolute = fpath.string();
		std::fstream file(absolute, std::fstream::out | std::ios::trunc);
		file.write(dataAsStr.data(), dataAsStr.size());
		file.flush();
		file.close();
	}

	bool Werckmeister::fileExists(const Path &path) const
	{
		return boost::filesystem::exists(path);
	}

	sheet::VoicingStrategyPtr Werckmeister::getDefaultVoicingStrategy()
	{
		return getVoicingStrategy(SHEET_VOICING_STRATEGY_DEFAULT);
	}

	sheet::VoicingStrategyPtr Werckmeister::getVoicingStrategy(const fm::String &name)
	{
		sheet::VoicingStrategyPtr result;
		const Path *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr) {
			auto anw = std::make_shared<sheet::compiler::LuaVoicingStrategy>(*scriptPath);
			try {
				anw->assertCanExecute();
			} catch (const Exception &ex) {
				fm::StringStream ss;
				ss << "'" << *scriptPath << "'" << ": failed to execute script:" << std::endl;
				ss << "  "  << ex.what();
				FM_THROW(Exception, ss.str());
			}
			result = anw;
			result->name(name);
			return result;
		}
		result = solve<sheet::VoicingStrategy>(name);
		result->name(name);
		return result;
	}

	sheet::compiler::AModificationPtr Werckmeister::getSpielanweisung(const fm::String &name)
	{
		return getModification(name);
	}

	sheet::compiler::AModificationPtr Werckmeister::getModification(const fm::String &name)
	{	
		const Path *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr) {
			auto anw = std::make_shared<sheet::compiler::LuaModification>(*scriptPath);
			try {
				anw->assertCanExecute();
			} catch (const Exception &ex) {
				fm::StringStream ss;
				ss << "'" << *scriptPath << "'" << ": failed to execute script:" << std::endl;
				ss << "  "  << ex.what();
				FM_THROW(Exception, ss.str());
			}
			return anw;
		}
		
		auto result = solve<sheet::compiler::AModification>(name);
		return result;
	}

	void Werckmeister::registerLuaScript(const Path &path)
	{
		auto fpath = boost::filesystem::system_complete(path);
		if(!boost::filesystem::exists(fpath)) {
			FM_THROW(Exception, "file does not exists " + path);
		}
		auto name = boost::filesystem::path(path).filename().stem().string();
		_scriptMap[name] = path;
	}

	const Path * Werckmeister::findScriptPathByName(const fm::String &name) const
	{
		auto it = _scriptMap.find(name);
		if (it == _scriptMap.end()) {
			return nullptr;
		}
		return &it->second;
	}

	Path Werckmeister::resolvePath(const Path &strRelPath) const
	{
		auto rel = boost::filesystem::path(strRelPath);
		if (rel.is_absolute()) {
			if (!boost::filesystem::exists(rel)) {
				FM_THROW(Exception, fm::String("could not resolve " + strRelPath));
			}
			return strRelPath;
		}
		for (const auto &searchPath : _searchPaths) {
			auto base = boost::filesystem::path(searchPath);
			auto x = boost::filesystem::absolute(rel, base);
			if (!boost::filesystem::exists(x)) {
				continue;
			}
			x = boost::filesystem::canonical(rel, base);
			return boost::filesystem::system_complete(x).string();
		}
		auto strSearchPaths = boost::algorithm::join(_searchPaths, "\n");
		FM_THROW(Exception, fm::String("could not resolve " + strRelPath + "\nsearched here:\n" + strSearchPaths));
	}

	Path Werckmeister::absolutePath(const Path &relPath) const
	{
		return boost::filesystem::system_complete(relPath).string();
	}

	const Werckmeister::Paths & Werckmeister::searchPaths() const
	{
		return _searchPaths;
	}

	void Werckmeister::addSearchPath(const Path & searchPath)
	{
		auto path = boost::filesystem::path(searchPath);
		if (!boost::filesystem::is_directory(path)) {
			path = path.parent_path();
		}
		_searchPaths.push_front(path.string());
	}

	const Werckmeister::CreateContextFunction & Werckmeister::createContextHandler() const
	{
		return this->_createContextHandler;
	}

	void Werckmeister::createContextHandler(const CreateContextFunction &createContextHandler)
	{
		this->_createContextHandler = createContextHandler;
	}

	bool Werckmeister::fileIsSheet(const Path &path) const 
	{
		return boost::filesystem::extension(path) == ".sheet";
	}

	Werckmeister::~Werckmeister() = default;
}
