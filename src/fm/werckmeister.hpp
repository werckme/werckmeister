#ifndef FM_WERKMEISTER_HPP
#define FM_WERKMEISTER_HPP

#include "units.hpp"
#include "config.hpp"
#include "fm/midi.hpp"
#include <fstream>
#include <memory>
#include "fm/common.hpp"
#include "forward.hpp"
#include <map>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <loki/Singleton.h>
#pragma GCC diagnostic pop
#else
#include <loki/Singleton.h>
#endif


namespace fm {
    class Werckmeister {
        friend struct Loki::CreateUsingNew<Werckmeister>;
    public:
        Werckmeister() {}
		Werckmeister(const Werckmeister&&) = delete;
		Werckmeister& operator=(const Werckmeister&&) = delete;
		typedef std::ifstream StreamType;
		typedef std::unique_ptr<StreamType> ResourceStream;
		typedef std::vector<fm::String> Paths;
        /*
            creates a default sheet. if werk parameter set, it will added to the werk after creation.
        */
		midi::MidiPtr createMidi();
        virtual ~Werckmeister();
        const char * version() const;
		sheet::compiler::CompilerPtr createCompiler();
		sheet::compiler::AContextPtr createContext();
		sheet::compiler::ASpielanweisungPtr getDefaultSpielanweisung();
		sheet::compiler::ASpielanweisungPtr getSpielanweisung(const fm::String &name);
		sheet::compiler::AModificationPtr getModification(const fm::String &name);		
		sheet::VoicingStrategyPtr getDefaultVoicingStrategy();
		sheet::VoicingStrategyPtr getVoicingStrategy(const fm::String &name);
		sheet::DocumentPtr createDocument();
		void registerLuaScript(const fm::String &path);
		const Paths & searchPaths() const;
		void addSearchPath(const fm::String &path);

	private:
		typedef std::unordered_map<fm::String, fm::String> ScriptMap;
		ScriptMap _scriptMap;
		ResourceStream openResourceImpl(const fm::String &path);
		Paths _searchPaths;
	public:
		fm::String resolvePath(const fm::String &relPath, sheet::ConstDocumentPtr, const fm::String &sourcePath = FM_STRING("")) const;
		const fm::String * findScriptPathByName(const fm::String &name) const;
		ResourceStream openResource(const std::string &path)
		{
			return openResourceImpl(path);
		}
		void saveResource(const fm::String &path, const fm::String &data);
    };
    Werckmeister & getWerckmeister();
}

#endif