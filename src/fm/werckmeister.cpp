#include "werckmeister.hpp"
#include "midi.hpp"
#include "config.hpp"
#include <type_traits>

namespace fm {
    
	
	Werckmeister & getWerckmeister()
    {
        typedef Loki::SingletonHolder<Werckmeister> Holder;
		return Holder::Instance();
    }

    const char * Werckmeister::version() const
    {
        return FETZER_VERSION;
    }

	midi::MidiPtr Werckmeister::createMidi()
	{
		return std::make_shared<midi::Midi>(PPQ);
	}

	Werckmeister::~Werckmeister() = default;
}