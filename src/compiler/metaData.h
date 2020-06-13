#ifndef COMPILER_METADATA_H
#define COMPILER_METADATA_H

#include <sheet/objects/Event.h>
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include <sheet/objects/ChordDef.h>
#include <fm/IDefinitionsServer.h>
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include <list>
#include "forward.hpp"

namespace sheet {
    namespace compiler {
        struct TrackMetaData {
            fm::String instrument;
            fm::String uname;
            virtual ~TrackMetaData() = default;
        };
        struct VoiceMetaData {
            typedef std::set<PitchDef> PitchDefSet;
            static const fm::Ticks DefaultDuration;
			static const fm::Ticks DefaultBarLength;
            typedef std::map<PitchDef, fm::Ticks> WaitForTieBuffer;
            typedef std::list<ASpielanweisungPtr> Spielanweisungen;
            typedef std::list<AModificationPtr> Modifications;
            typedef std::unordered_map<fm::String, AModificationPtr> ModificationCache;
            fm::Ticks position = 0;
            fm::Ticks barLength = DefaultBarLength;
            int signatureNumerator = 4;
            int signatureDenominator = 4;
            fm::Ticks barPosition = 0;
            double tempoFactor = 1;
            int barCount = 0;
            long long eventCount = 0;
            long long eventOffset = 0;
            fm::Expression expression = fm::expression::FF;
            fm::Expression expressionPlayedOnce = fm::expression::Default;
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
}

#endif