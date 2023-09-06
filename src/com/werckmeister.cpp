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
#include "compiler/modification/EventFunction.h"
#include <documentModel/Document.h>
#include <locale>

namespace com
{
	namespace 
	{
		typedef std::unordered_map<com::String, compiler::AModificationPtr> ModCache;
		typedef std::unordered_map<com::String, compiler::LuaEventFunctionPtr> EventFunctionCache;
		typedef std::unordered_map<com::String, compiler::VoicingStrategyPtr> VoicingStrategyCache;
		ModCache _modCache;
		VoicingStrategyCache _voicingStrategyCache;
		EventFunctionCache _eventFunctionCache;

		template<class TMap>
		typename TMap::mapped_type _find(TMap &map, const com::String& key)
		{
			auto it = map.find(key);
			if (it == map.end())
			{
				return nullptr;
			}
			return it->second;
		}

		template<class TMap>
		void _register(TMap &map, const com::String& key, typename TMap::mapped_type value)
		{
			map.insert(std::make_pair(key, value));
		}

		compiler::AModificationPtr _findMod(const com::String& key)
		{
			return _find(_modCache, key);
		}

		compiler::LuaEventFunctionPtr _findEventFunction(const com::String& key)
		{
			return _find(_eventFunctionCache, key);
		}

		void _registerMod(const com::String& key, compiler::AModificationPtr mod)
		{
			return _register(_modCache, key, mod);
		}

		void _registerEventFunction(const com::String& key, compiler::LuaEventFunctionPtr eventFunction)
		{
			return _register(_eventFunctionCache, key, eventFunction);
		}

		compiler::VoicingStrategyPtr _findStrategy(const com::String& key)
		{
			return _find(_voicingStrategyCache, key);
		}

		void _registerStrategy(const com::String& key, compiler::VoicingStrategyPtr strategy)
		{
			return _register(_voicingStrategyCache, key, strategy);
		}
	}

	Werckmeister &getWerckmeister()
	{
		static Werckmeister instance;
		return instance;
	}

	const char *Werckmeister::version() const
	{
		return SHEET_VERSION;
	}

	midi::MidiPtr Werckmeister::createMidi()
	{
		return std::make_shared<midi::Midi>(PPQ);
	}

	Werckmeister::ResourceStream Werckmeister::openResourceImpl(const Path &path)
	{
		if (path.empty())
		{
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

	void Werckmeister::saveResource(const Path &path, const com::String &dataAsStr)
	{
		if (path.empty())
		{
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

	compiler::VoicingStrategyPtr Werckmeister::getDefaultVoicingStrategy()
	{
		return getVoicingStrategy(SHEET_VOICING_STRATEGY_DEFAULT);
	}

	compiler::VoicingStrategyPtr Werckmeister::getVoicingStrategy(const com::String& name, const String& uniqueCallerId)
	{
		if (uniqueCallerId.empty())
		{
			return getVoicingStrategy(name);
		}
		auto cacheKey = name + uniqueCallerId;
		auto strategy = _findStrategy(cacheKey);
		if (strategy != nullptr)
		{
			return strategy;
		}
		strategy = getVoicingStrategy(name);
		_registerStrategy(cacheKey, strategy);
		return strategy;
	}

	compiler::VoicingStrategyPtr Werckmeister::getVoicingStrategy(const com::String &name)
	{
		compiler::VoicingStrategyPtr result;
		const Path *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr)
		{
			auto anw = std::make_shared<compiler::LuaVoicingStrategy>(*scriptPath);
			try
			{
				anw->assertCanExecute();
			}
			catch (const Exception &ex)
			{
				com::StringStream ss;
				ss << "'" << *scriptPath << "'"
				   << ": failed to execute script:" << std::endl;
				ss << "  " << ex.what();
				FM_THROW(Exception, ss.str());
			}
			result = anw;
			result->name(name);
			return result;
		}
		result = solve<compiler::VoicingStrategy>(name);
		result->name(name);
		return result;
	}

	compiler::AModificationPtr Werckmeister::getSpielanweisung(const com::String &name)
	{
		return getModification(name);
	}

	compiler::AModificationPtr Werckmeister::getSpielanweisung(const com::String& name, const String& uniqueCallerId)
	{
		return getModification(name, uniqueCallerId);
	}

	compiler::AModificationPtr Werckmeister::getModification(const com::String& name, const String& uniqueCallerId)
	{
		if (uniqueCallerId.empty())
		{
			return getModification(name);
		}
		auto cacheKey = name + uniqueCallerId;
		auto mod = _findMod(cacheKey);
		if (mod != nullptr)
		{
			return mod;
		}
		mod = getModification(name);
		_registerMod(cacheKey, mod);
		return mod;
	}

	compiler::AModificationPtr Werckmeister::getModification(const com::String &name)
	{
		const Path *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr)
		{
			auto anw = std::make_shared<compiler::LuaModification>(*scriptPath);
			try
			{
				anw->assertCanExecute();
			}
			catch (const Exception &ex)
			{
				com::StringStream ss;
				ss << "'" << *scriptPath << "'"
				   << ": failed to execute script:" << std::endl;
				ss << "  " << ex.what();
				FM_THROW(Exception, ss.str());
			}
			return anw;
		}

		auto result = solve<compiler::AModification>(name);
		return result;
	}

	compiler::LuaEventFunctionPtr Werckmeister::getEventFunction(const com::String& name, const String& uniqueCallerId)
	{
		if (uniqueCallerId.empty())
		{
			return getEventFunction(name);
		}
		auto cacheKey = name + uniqueCallerId;
		auto eventFunction = _findEventFunction(cacheKey);
		if (eventFunction != nullptr)
		{
			return eventFunction;
		}
		eventFunction = getEventFunction(name);
		_registerEventFunction(cacheKey, eventFunction);
		return eventFunction;
	}

	compiler::LuaEventFunctionPtr Werckmeister::getEventFunction(const com::String &name)
	{
		const Path *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr)
		{
			auto anw = std::make_shared<compiler::LuaEventFunction>(*scriptPath);
			try
			{
				anw->assertCanExecute();
			}
			catch (const Exception &ex)
			{
				com::StringStream ss;
				ss << "'" << *scriptPath << "'"
				   << ": failed to execute script:" << std::endl;
				ss << "  " << ex.what();
				FM_THROW(Exception, ss.str());
			}
			return anw;
		}

		auto result = solve<compiler::LuaEventFunction>(name);
		return result;
	}

	void Werckmeister::clearCache()
	{
		_modCache.clear();
		_voicingStrategyCache.clear();
		_eventFunctionCache.clear();
	}

	void Werckmeister::registerLuaScript(const Path &path)
	{
		auto fpath = boost::filesystem::system_complete(path);
		if (!boost::filesystem::exists(fpath))
		{
			FM_THROW(Exception, "file does not exists " + path);
		}
		auto name = boost::filesystem::path(path).filename().stem().string();
		_scriptMap[name] = path;
	}

	const Path *Werckmeister::findScriptPathByName(const com::String &name) const
	{
		auto it = _scriptMap.find(name);
		if (it == _scriptMap.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	namespace 
	{
		bool isAbsoluteWMPath(const Path& path)
		{
			static std::locale loc;
			for (auto _char : path)
			{
				if (std::isspace(_char, loc))
				{
					continue;
				}
				if (_char == '/') 
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			return false;
		}
	}

	Path Werckmeister::resolvePath(const Path &strRelPath) const
	{
		auto rel = boost::filesystem::path(strRelPath);
		if (isAbsoluteWMPath(strRelPath))
		{
			if (boost::filesystem::exists(rel))
			{
				return strRelPath;
			}
			rel = boost::filesystem::path("." + strRelPath); // #212
		}
		for (const auto &searchPath : _searchPaths)
		{
			auto base = boost::filesystem::path(searchPath);
			auto x = boost::filesystem::absolute(rel, base);
			if (!boost::filesystem::exists(x))
			{
				continue;
			}
			x = boost::filesystem::canonical(rel, base);
			return boost::filesystem::system_complete(x).string();
		}
		auto strSearchPaths = boost::algorithm::join(_searchPaths, "\n");
		FM_THROW(Exception, com::String("could not find " + strRelPath + "\nsearched here:\n" + strSearchPaths));
	}

	Path Werckmeister::absolutePath(const Path &relPath) const
	{
		return boost::filesystem::system_complete(relPath).string();
	}

	const Werckmeister::Paths &Werckmeister::searchPaths() const
	{
		return _searchPaths;
	}

	void Werckmeister::addSearchPath(const Path &searchPath)
	{
		if (searchPath.empty()) 
		{
			return;
		}
		auto path = boost::filesystem::path(searchPath);
		if (!boost::filesystem::is_directory(path))
		{
			path = path.parent_path();
		}
		_searchPaths.push_front(path.string());
	}

	const Werckmeister::CreateContextFunction &Werckmeister::createContextHandler() const
	{
		return this->_createContextHandler;
	}

	void Werckmeister::createContextHandler(const CreateContextFunction &createContextHandler)
	{
		this->_createContextHandler = createContextHandler;
	}

	bool Werckmeister::fileIsSheet(const Path &path) const
	{
		return boost::filesystem::extension(path) == ".documentModel";
	}

	Werckmeister::~Werckmeister() = default;
}
