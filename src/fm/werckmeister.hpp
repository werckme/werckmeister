#ifndef FM_WERKMEISTER_HPP
#define FM_WERKMEISTER_HPP

#include "units.hpp"
#include "config.hpp"
#include "fm/midi.hpp"

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

        /*
            creates a default sheet. if werk parameter set, it will added to the werk after creation.
        */
		midi::MidiPtr createMidi();
        virtual ~Werckmeister();
        const char * version() const;
    };
    Werckmeister & getWerckmeister();
}

#endif