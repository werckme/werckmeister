#ifndef COMPILER_SHEET_TEMPLATERENDERER_H
#define COMPILER_SHEET_TEMPLATERENDERER_H

#include <sheet/objects/Event.h>
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include <sheet/objects/ChordDef.h>
#include <fm/SheetTemplateDefServer.h>
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"
#include "context/AContext.h"

namespace sheet {
    namespace compiler {
        class SheetEventRenderer;
        class SheetTemplateRenderer {
        public:
            SheetTemplateRenderer(AContext* ctx, SheetEventRenderer *renderer);
            virtual ~SheetTemplateRenderer();
            void render(Track * sheetTrack);
            AContext* context() const { return this->ctx_; }
            SheetEventRenderer *sheetEventRenderer;
        private:
            void setTargetCreateIfNotExists(const Track &track, const Voice &voice);
            typedef std::unordered_map<const void*, AContext::Id> PtrIdMap;
            AContext* ctx_;
			PtrIdMap ptrIdMap_;
        };
    }
}

#endif