#ifndef COMPILER_STYLERENDERER_H
#define COMPILER_STYLERENDERER_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include "sheet/ChordDef.h"
#include "sheet/StyleDefServer.h"
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"
#include "AContext.h"

namespace sheet {
    namespace compiler {
        class StyleRenderer {
        public:
            StyleRenderer(AContextPtr ctx) : ctx_(ctx) {}
            virtual ~StyleRenderer() = default;
            void render(fm::Ticks duration);
            void sheetRest(fm::Ticks duration);
            void switchStyle(const IStyleDefServer::Style &current, const IStyleDefServer::Style &next);
            AContextPtr context() const { return this->ctx_; }
        private:
            void remberPosition(const Voice &voice, 
                const Event &ev, 
                Voice::Events::const_iterator it,
                fm::Ticks originalEventDuration
            );
            Voice::Events::const_iterator skipEvents(Voice::Events::const_iterator it, Voice::Events::const_iterator end, int n);
            Voice::Events::const_iterator continueOnRemeberedPosition(const Voice &voice);       
            inline bool allWritten(fm::Ticks totalDuration, fm::Ticks written) const 
            {
                return (totalDuration - written) <= AContext::TickTolerance;
            }
            inline bool hasRemberedPosition(const VoiceMetaData &meta) const {
                return meta.idxLastWrittenEvent >= 0;
            }
            void setTargetCreateIfNotExists(const Track &track, const Voice &voice);
            /**
             * @return the written duration
             */
            fm::Ticks renderVoice(const Voice &voice, 
                Voice::Events::const_iterator begin, 
                fm::Ticks totalDuration, 
                fm::Ticks alreadyWritten
            );
            typedef std::unordered_map<const void*, AContext::Id> PtrIdMap;
			PtrIdMap ptrIdMap_;
            AContextPtr ctx_;
        };
    }
}

#endif