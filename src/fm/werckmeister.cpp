#include "werckmeister.hpp"
#include "midi.hpp"
#include "config.hpp"
#include <type_traits>
#include <boost/filesystem.hpp>
#include <exception>
#include <fstream>
#include "compiler/compiler.h"
#include "compiler/MidiContext.h"
#include <memory>
#include "compiler/voicings/DirectVoicingStrategy.h"
#include "compiler/voicings/SimpleGuitar.h"
#include "compiler/voicingStrategies.h"
#include "compiler/voicings/VoicingStrategy.h"
#include "compiler/voicings/Lua.h"
#include "compiler/spielanweisung/Normal.h"
#include "compiler/spielanweisung/Arpeggio.h"
#include "compiler/spielanweisung/Vorschlag.h"
#include "compiler/spielanweisung/spielanweisungen.h"
#include "compiler/modification/modifications.h"
#include "compiler/modification/Bend.h"
#include <fm/exception.hpp>


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


	Werckmeister::ResourceStream Werckmeister::openResourceImpl(const fm::String &path)
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
		auto result = std::make_unique<StreamType>(absolute.c_str());
		return result;
	}

	sheet::compiler::CompilerPtr Werckmeister::createCompiler()
	{
		return std::make_shared<sheet::compiler::Compiler>();
	}

	sheet::compiler::AContextPtr Werckmeister::createContext()
	{
		auto midiContext = std::make_shared<sheet::compiler::MidiContext>();
		return midiContext;
	}

	sheet::compiler::ASpielanweisungPtr Werckmeister::getDefaultSpielanweisung()
	{
		return getSpielanweisung(SHEET_SPIELANWEISUNG_NORMAL);
	}

	sheet::VoicingStrategyPtr Werckmeister::getDefaultVoicingStrategy()
	{
		return getVoicingStrategy(SHEET_VOICING_STRATEGY_DEFAULT);
	}

	sheet::VoicingStrategyPtr Werckmeister::getVoicingStrategy(const fm::String &name)
	{
		sheet::VoicingStrategyPtr result;
		const fm::String *scriptPath = findScriptPathByName(name);
		if (scriptPath != nullptr) {
			auto anw = std::make_shared<sheet::compiler::LuaVoicingStrategy>(*scriptPath);
			if (anw->canExecute()) {
				anw->name(name);
				result = anw;
			}
		}
		if (name == SHEET_VOICING_STRATEGY_SIMPLE_GUITAR) {
			result = std::make_shared<sheet::SimpleGuitar>();
		}
		if (name == SHEET_VOICING_STRATEGY_AS_NOTATED) {
			result = std::make_shared<sheet::DirectVoicingStrategy>();
		}
		if (!result) {
			FM_THROW(Exception, "voicing strategy not found: " + fm::to_string(name));
		}
		result->name(name);
		return result;
	}

	sheet::compiler::ASpielanweisungPtr Werckmeister::getSpielanweisung(const fm::String &name)
	{
		if (name == SHEET_SPIELANWEISUNG_NORMAL) {
			return std::make_shared<sheet::compiler::Normal>(); 
		}
		if (name == SHEET_SPIELANWEISUNG_ARPEGGIO) {
			return std::make_shared<sheet::compiler::Arpeggio>(); 
		}
		if (name == SHEET_SPIELANWEISUNG_VORSCHLAG) {
			return std::make_shared<sheet::compiler::Vorschlag>(); 
		}		
		FM_THROW(Exception, "spielanweisung not found: " + fm::to_string(name));
	}

	sheet::compiler::AModificationPtr Werckmeister::getModification(const fm::String &name)
	{	
		if (name == SHEET_MOD_BEND) {
			return std::make_shared<sheet::compiler::Bend>();  
		}
		FM_THROW(Exception, "modification not found: " + fm::to_string(name));
	}

	void Werckmeister::registerLuaScript(const fm::String &path)
	{
		auto name = boost::filesystem::path(path).filename().stem().wstring();
		_scriptMap[name] = path;
	}

	const fm::String * Werckmeister::findScriptPathByName(const fm::String &name) const
	{
		auto it = _scriptMap.find(name);
		if (it == _scriptMap.end()) {
			return nullptr;
		}
		return &it->second;
	}

	Werckmeister::~Werckmeister() = default;
}
