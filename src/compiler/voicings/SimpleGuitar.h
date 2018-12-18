#ifndef SIMPLE_GUITAR_STATEGY_HPP
#define SIMPLE_GUITAR_STATEGY_HPP

#include "DirectVoicingStrategy.h"
#include <map>

namespace sheet {

    /**
     * A naive voicing implementation which approaches the chord voicing on a guitar:
     * I V I III: default chord
 	 * I V VII III: 7
	 * I III VII II V: 9
	 * I IV VII II V: 11
	 * I IV VII III VI: 13    
     */
    class SimpleGuitar : public DirectVoicingStrategy {
    public:
        typedef fm::Pitch Degree;
        typedef int Octave;
        typedef std::multimap<Degree, Octave> OctaveMap; 
		typedef DirectVoicingStrategy Base;
        virtual ~SimpleGuitar() = default;
		virtual Pitches get(const ChordEvent &chord, const ChordDef &def, const Degrees &degreeIntervals, const TimeInfo&) override;
        OctaveMap createOctaveMap(const ChordDef &def) const;
    };
}

#endif