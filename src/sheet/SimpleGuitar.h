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
        bool has7(const ChordDef &def) const;
        bool has9(const ChordDef &def) const;
        bool has11(const ChordDef &def) const;
        bool has13(const ChordDef &def) const;
        OctaveMap createOctaveMap(const ChordDef &def) const;
    };
}

#endif