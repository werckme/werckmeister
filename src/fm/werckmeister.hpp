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
		typedef std::istream StreamType;
		typedef std::unique_ptr<StreamType> ResourceStream;
		typedef std::vector<Path> Paths;
        /*
            creates a default sheet. if werk parameter set, it will added to the werk after creation.
        */
		midi::MidiPtr createMidi();
        virtual ~Werckmeister();
        const char * version() const;
		sheet::compiler::CompilerPtr createCompiler();
		sheet::compiler::AContextPtr createContext();
		sheet::compiler::ASpielanweisungPtr getDefaultSpielanweisung();
		sheet::compiler::ASpielanweisungPtr getSpielanweisung(const String &name);
		sheet::compiler::AModificationPtr getModification(const String &name);		
		sheet::VoicingStrategyPtr getDefaultVoicingStrategy();
		sheet::VoicingStrategyPtr getVoicingStrategy(const String &name);
		sheet::DocumentPtr createDocument();
		void registerLuaScript(const Path &path);
		const Paths & searchPaths() const;
		void addSearchPath(const Path &path);
		Path createVirtualFile();
		const String * getVirtualFileData(const Path &path) const;
		void updateVirtualFile(const Path &path, const String &data);
		bool isVirtualFilePath(const Path &path) const;
		ResourceStream openVirtualFile(const Path &path);

	private:
		typedef std::unordered_map<fm::String, Path> ScriptMap;
		typedef std::unordered_map<Path, String> VirtualFiles;
		VirtualFiles virtualFiles_;
		ScriptMap _scriptMap;
		ResourceStream openResourceImpl(const Path &path);
		Paths _searchPaths;
	public:
		Path resolvePath(const Path &relPath, sheet::ConstDocumentPtr, const Path &sourcePath = FM_STRING("")) const;
		Path absolutePath(const Path &relPath) const;
		bool fileExists(const Path &path) const;
		const Path * findScriptPathByName(const fm::String &name) const;
		ResourceStream openResource(const std::string &path)
		{
			return openResourceImpl(path);
		}
		void saveResource(const Path &path, const fm::String &data);
    };
    Werckmeister & getWerckmeister();
}

#endif