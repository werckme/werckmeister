#pragma once

#include <documentModel/objects/Event.h>
#include <memory>
#include <unordered_map>
#include <com/units.hpp>
#include <com/literals.hpp>
#include <map>
#include <unordered_map>
#include <documentModel/objects/ChordDef.h>
#include "compiler/voicings/VoicingStrategy.h"
#include <com/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"
#include "context/IContext.h"
#include "ISheetTemplateRenderer.h"
#include "ASheetEventRenderer.h"
#include "ICompilerVisitor.h"
#include "IDefinitionsServer.h"

namespace compiler
{

    class SheetTemplateRenderer : public ISheetTemplateRenderer
    {
    public:
        SheetTemplateRenderer(IContextPtr ctx, ASheetEventRendererPtr renderer, ICompilerVisitorPtr compilerVisitor, IDefinitionsServerPtr definitionsServer);
        virtual ~SheetTemplateRenderer();
        void render(documentModel::Track *sheetTrack);
        IContextPtr context() const { return this->_ctx; }
        ASheetEventRendererPtr sheetEventRenderer;
    private:
        typedef com::String ContainerKeyType;
        void setTargetCreateIfNotExists(const documentModel::Track &track, const documentModel::Voice &voice);
        typedef std::unordered_map<com::String, IContext::Id> Container2ContextElementId;
        ContainerKeyType getKey(const documentModel::Track &) const;
        ContainerKeyType getKey(const documentModel::Voice &) const;
        IContextPtr _ctx;
        Container2ContextElementId _contextElementIdMap;
        ICompilerVisitorPtr _compilerVisitor;
        IDefinitionsServerPtr definitionsServer_;
    };
}
