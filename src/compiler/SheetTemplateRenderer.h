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
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"
#include "context/IContext.h"
#include "ISheetTemplateRenderer.h"
#include "ASheetEventRenderer.h"

namespace sheet {
    namespace compiler {

        class SheetTemplateRenderer : public ISheetTemplateRenderer {
        public:
            SheetTemplateRenderer(IContextPtr ctx, ASheetEventRendererPtr renderer);
            virtual ~SheetTemplateRenderer();
            void render(Track * sheetTrack);
            IContextPtr context() const { return this->ctx_; }
            ASheetEventRendererPtr sheetEventRenderer;
        private:
            typedef fm::String ContainerKeyType;
            void setTargetCreateIfNotExists(const Track &track, const Voice &voice);
            typedef std::unordered_map<fm::String, IContext::Id> Container2ContextElementId;
            ContainerKeyType getKey(const Track&) const;
            ContainerKeyType getKey(const Voice&) const;
            IContextPtr ctx_;
			Container2ContextElementId _contextElementIdMap;
        };
    }
}

#endif