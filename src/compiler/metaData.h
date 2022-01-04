#pragma once

#include <documentModel/objects/Event.h>
#include <memory>
#include <unordered_map>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <map>
#include <unordered_map>
#include <documentModel/objects/ChordDef.h>
#include <compiler/IDefinitionsServer.h>
#include "compiler/voicings/VoicingStrategy.h"
#include <com/common.hpp>
#include <list>
#include "forward.hpp"

namespace compiler
{
    struct TrackMetaData
    {
        com::String instrument;
        com::String uname;
        virtual ~TrackMetaData() = default;
    };
    struct VoiceMetaData
    {
        typedef std::set<documentModel::PitchDef> PitchDefSet;
        static const com::Ticks DefaultDuration;
        static const com::Ticks DefaultBarLength;
        typedef std::map<documentModel::PitchDef, com::Ticks> WaitForTieBuffer;
        typedef std::list<ASpielanweisungPtr> Spielanweisungen;
        typedef std::list<AModificationPtr> Modifications;
        typedef std::unordered_map<com::String, AModificationPtr> ModificationCache;
        com::Ticks position = 0;
        com::Ticks barLength = DefaultBarLength;
        int signatureNumerator = 4;
        int signatureDenominator = 4;
        com::Ticks barPosition = 0;
        double tempoFactor = 1;
        int barCount = 0;
        long long eventCount = 0;
        long long eventOffset = 0;
        com::Expression expression = com::expression::FF;
        com::Expression expressionPlayedOnce = com::expression::Default;
        WaitForTieBuffer waitForTieBuffer;
        VoicingStrategyPtr voicingStrategy = nullptr;
        virtual ~VoiceMetaData() = default;
        bool pendingTie() const { return !waitForTieBuffer.empty(); }
        AModificationPtr spielanweisung;
        AModificationPtr spielanweisungOnce; // played once
        Modifications modifications;
        Modifications modificationsOnce; // played once
        ModificationCache modificationCache;
        PitchDefSet startedEvents;
        double volume = 100;
        double pan = 50;
    };
}
