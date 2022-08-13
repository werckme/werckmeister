#pragma once

#include <com/common.hpp>
#include <com/units.hpp>
#include <set>
#include "Interval.h"
#include "ASheetObject.h"
#include <documentModel/objects/DegreeDef.h>

namespace documentModel
{
    struct PitchDef;
    struct ChordDef : public ASheetObject
    {
        typedef std::set<DegreeDef> DegreeDefs;
        com::String name;
        DegreeDefs degreeDefs;
        DegreeDef findDegreeDef(const PitchDef &eventPitch) const;
    };

    bool has7(const ChordDef &def);
    bool has9(const ChordDef &def);
    bool has11(const ChordDef &def);
    bool has13(const ChordDef &def);
    int getDegreeValue(int degree);
    com::degrees::Flag getFlag(int degree);
}
