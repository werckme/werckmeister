#pragma once

#include "units.hpp"
#include "config.hpp"
#include <fstream>
#include <memory>
#include "com/common.hpp"
#include "forward.hpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <functional>
#include <com/IRegisterable.h>
#include <com/exception.hpp>
#include <list>

namespace com {
    class Werckmeister {
    public:
        Werckmeister() {}
		Werckmeister(const Werckmeister&&) = delete;
		Werckmeister& operator=(const Werckmeister&&) = delete;
		typedef std::istream StreamType;
		typedef std::unique_ptr<StreamType> ResourceStream;
		typedef std::list<Path> Paths;
		typedef std::function<documentModel::compiler::AContextPtr()> CreateContextFunction;
        /*
            creates a default documentModel. if werk parameter set, it will added to the werk after creation.
        */
		midi::MidiPtr createMidi();
        virtual ~Werckmeister();
        const char * version() const;
		documentModel::compiler::AModificationPtr getSpielanweisung(const String &name);
		documentModel::compiler::AModificationPtr getModification(const String &name);		
		documentModel::VoicingStrategyPtr getDefaultVoicingStrategy();
		documentModel::VoicingStrategyPtr getVoicingStrategy(const String &name);
		void registerLuaScript(const Path &path);
		const Paths & searchPaths() const;
		void addSearchPath(const Path &path);
		const CreateContextFunction & createContextHandler() const;
		void createContextHandler(const CreateContextFunction &createContextHandler);
	private:
		typedef std::unordered_map<com::String, Path> ScriptMap;
		typedef std::function<std::shared_ptr<com::IRegisterable>()> CreateIRegFunction;
		typedef std::unordered_map<com::String, CreateIRegFunction> FactoryMap;
		FactoryMap _factoryMap;
		ScriptMap _scriptMap;
		ResourceStream openResourceImpl(const Path &path);
		Paths _searchPaths;
		CreateContextFunction _createContextHandler;
	public:
		template<class TRegisterable>
		bool register_(const com::String &name, const CreateIRegFunction&);
		template<class TRegisterable>
		bool replaceRegistration(const com::String &name, const CreateIRegFunction&);
		template<class TRegisterable>
		std::shared_ptr<TRegisterable> solve(const com::String &name);
		template<class TRegisterable>
		std::shared_ptr<TRegisterable> solveOrDefault(const com::String &name);
		Path resolvePath(const Path &relPath) const;
		Path absolutePath(const Path &relPath) const;
		bool fileExists(const Path &path) const;
		bool fileIsSheet(const Path &path) const;
		const Path * findScriptPathByName(const com::String &name) const;
		ResourceStream openResource(const std::string &path)
		{
			return openResourceImpl(path);
		}
		void saveResource(const Path &path, const com::String &data);

    };
    Werckmeister & getWerckmeister();
	///////////////////////////////////////////////////////////////////////////
	template<class TRegisterable>
	bool Werckmeister::register_(const com::String &name, const CreateIRegFunction &create)
	{
		return _factoryMap.emplace(std::make_pair(name, create)).second;
	}
	template<class TRegisterable>
	bool Werckmeister::replaceRegistration(const com::String &name, const CreateIRegFunction &create)
	{
		_factoryMap[name] = create;
		return true;
	}	
	template<class TRegisterable>
	std::shared_ptr<TRegisterable> Werckmeister::solveOrDefault(const com::String &name)
	{
		auto it = _factoryMap.find(name);
		if (it == _factoryMap.end()) {
			return nullptr;
		}
		auto ptr = it->second();
		std::shared_ptr<TRegisterable> result = std::dynamic_pointer_cast<TRegisterable>(ptr);
		if (!ptr) {
			FM_THROW(Exception, "Failed to create '" + name + "': wrong type expected.");
		}
		return result;
	}
	template<class TRegisterable>
	std::shared_ptr<TRegisterable> Werckmeister::solve(const com::String &name)
	{
		auto result = this->solveOrDefault<TRegisterable>(name);
		if (!result) {
			FM_THROW(Exception, name + " not found.");
		}
		return result;
	}	
}

