#pragma once

#include "DirectVoicingStrategy.h"
#include <map>
#include <compiler/argumentNames.h>
namespace compiler
{

    /**
     * A naive voicing implementation which approaches the chord voicing on a guitar:
     * I V I III: default chord
 	 * I V VII III: 7
	 * I III VII II V: 9
	 * I IV VII II V: 11
	 * I IV VII III VI: 13    
     */
    class SimpleGuitar : public DirectVoicingStrategy
    {
    public:
        typedef com::Pitch Degree;
        typedef int Octave;
        typedef std::multimap<Degree, Octave> OctaveMap;
        typedef DirectVoicingStrategy Base;
        virtual ~SimpleGuitar() = default;
        virtual Pitches solve(const documentModel::Event &chord, const documentModel::ChordDef &def, const Pitches &degreePitches, const TimeInfo &) override;
        OctaveMap createOctaveMap(const documentModel::ChordDef &def) const;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEFAULT_DEF(argumentNames.SimpleGuitarVoicingStrategy.Range, 0, "higherRange"),
        };
        virtual ParametersByNames &getParameters() { return this->parameters; }
    };
}
