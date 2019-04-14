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
		typedef std::wifstream StreamType;
		typedef std::unique_ptr<StreamType> ResourceStream;

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
	private:
		typedef std::unordered_map<fm::String, fm::String> ScriptMap;
		ScriptMap _scriptMap;
		ResourceStream openResourceImpl(const fm::String &path);
	public:
		const fm::String * findScriptPathByName(const fm::String &name) const;
		ResourceStream openResource(const std::string &path)
		{
			return openResourceImpl(fm::to_wstring(path));
		}
		ResourceStream openResource(const std::wstring &path)
		{
			return openResourceImpl(path);
		}
		void saveResource(const std::wstring &path, const fm::String &data);
		void saveResource(const std::string &path, const fm::String &data)
		{
			saveResource(fm::to_wstring(path), data);
		}
    };
    Werckmeister & getWerckmeister();
}

#endif