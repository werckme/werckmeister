#include "ExecuteEventFunction.h"
#include <compiler/context/IContext.h>
#include <compiler/modification/EventFunction.h>
#include <com/werckmeister.hpp>
#include <algorithm>
#include <compiler/error.hpp>


namespace compiler
{
    void ExecuteEventFunction::execute(IContextPtr context)
    {
        if (context->getCompilerState() != IContext::CompilerState::RenderTrack 
            && context->getCompilerState() != IContext::CompilerState::RenderAccompTrackChords) 
        {
            return;
        }
        com::String modName = parameters[argumentNames.AddMod.Use].value<com::String>();
        auto meta = context->voiceMetaData();
        auto theEventFunction = loadEventFunction(context, modName);
        AModification::Events events;
        theEventFunction->execute(context, events);
        const auto &originEvent = event();
        com::Ticks passedTicks = 0;
        for(auto &event : events)
        {
            event.sourceId =  originEvent.sourceId;
            event.sourcePositionBegin = originEvent.sourcePositionBegin;
            event.sourcePositionEnd = originEvent.sourcePositionEnd;
            sheetEventRenderer_->addEvent(event);
            passedTicks += event.duration;
        }
        context->seek(-passedTicks);
    }

    LuaEventFunctionPtr ExecuteEventFunction::loadEventFunction(IContextPtr ctx, const com::String& modName)
    {
        auto& wm = com::getWerckmeister();
        auto voiceId = ctx->voice();
        LuaEventFunctionPtr theEventFunction = nullptr;
        if (voiceId != compiler::IContext::INVALID_VOICE_ID) {
            auto uniqueCallerId = std::to_string(voiceId);
            theEventFunction = wm.getEventFunction(modName, uniqueCallerId);
        }
        else
        {
            theEventFunction = wm.getEventFunction(modName);
        }
        theEventFunction->setArguments(modArgs);
        return theEventFunction;
    }
}