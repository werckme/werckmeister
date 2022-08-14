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
        DegreeDef resolveDegreeDef(const PitchDef &eventPitch) const;
        const DegreeDef* findDegreeDef(int degreeValue) const;
    };

    bool has7(const ChordDef &def);
    bool has9(const ChordDef &def);
    bool has11(const ChordDef &def);
    bool has13(const ChordDef &def);
    int getDegreeValue(int degreeValueWithBitshiftetFlags);
    com::degrees::Flag getFlag(int degree);
}
