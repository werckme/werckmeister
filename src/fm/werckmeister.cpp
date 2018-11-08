#include "werckmeister.hpp"
#include "midi.hpp"
#include "config.hpp"
#include <type_traits>
#include <boost/filesystem.hpp>
#include <exception>
#include <fstream>
#include "compiler/compiler.h"
#include "compiler/MidiContext.h"

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
		auto fpath = boost::filesystem::system_complete(path);
		auto absolute = fpath.string();
		if (!boost::filesystem::exists(path))
		{
			throw std::runtime_error("resource not found: " + fpath.string());
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

	Werckmeister::~Werckmeister() = default;
}