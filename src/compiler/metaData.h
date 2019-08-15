#ifndef COMPILER_METADATA_H
#define COMPILER_METADATA_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include "sheet/ChordDef.h"
#include "sheet/SheetTemplateDefServer.h"
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
            fm::Ticks position = 0;
            /**
             * last note duration
             */
            fm::Ticks lastEventDuration = DefaultDuration; 
            fm::Ticks barLength = DefaultBarLength;
            int signatureNumerator = 4;
            int signatureDenominator = 4;
            fm::Ticks barPosition = 0;
            int barCount = 0;
            long long eventCount = 0;
            long long eventOffset = 0;
            bool isUpbeat = false;
            fm::Expression expression = fm::expression::FF;
            fm::Expression singleExpression = fm::expression::Default;
            WaitForTieBuffer waitForTieBuffer;
            /*
                used for continue sheetTemplate track rendering after chord change
            */
            int idxLastWrittenEvent = -1;
            /*
                from a aborted sheetTemplate rendering
            */
            fm::Ticks remainingTime = 0;
            VoicingStrategyPtr voicingStrategy = nullptr;
            virtual ~VoiceMetaData() = default;
            bool pendingTie() const { return !waitForTieBuffer.empty(); }
            AModificationPtr spielanweisung;
            AModificationPtr spielanweisungOnce; // played once
            Modifications modifications;
            Modifications modificationsOnce; // played once		
            PitchDefSet startedEvents;
            int volume = 100;	
            int pan = 50;            
        };
    }
}

#endif