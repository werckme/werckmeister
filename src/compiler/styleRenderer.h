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
            void setTargetCreateIfNotExists(const Track &track, const Voice &voice);
            typedef std::unordered_map<const void*, AContext::Id> PtrIdMap;
			PtrIdMap ptrIdMap_;
            AContextPtr ctx_;
        };
    }
}

#endif