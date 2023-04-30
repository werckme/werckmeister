#include "PhraseRenderer.h"
#include <documentModel/objects/Event.h>
#include <compiler/error.hpp>

namespace compiler
{
    void PhraseRenderer::renderPhrase(const documentModel::Event &phraseEvent)
    {
        try 
        {
            renderImpl(phraseEvent);
        }
        catch (com::Exception &ex)
		{
			ex << ex_sheet_source_info(phraseEvent);
			throw;
		}
    }
    void PhraseRenderer::renderImpl(const documentModel::Event &phraseEvent)
    {
        auto phraseName = phraseEvent.phraseName();
        logger_->babble(WMLogLambda(log << "render phrase '" << phraseName << "'"));
        auto phraseInfo = definitionServer_->getPhrase(phraseName);
        if(phraseInfo.events == nullptr)
        {
            FM_THROW(Exception, "phrase not found: " + phraseName);
        }
        const auto &phraseEvents = *phraseInfo.events;
        logger_->babble(WMLogLambda(log << "phrase found: " << phraseEvents.size() << " events"));
        for(const auto &event : phraseEvents)
        {
            auto copy = event;
            copy.duration = event.duration * (phraseEvent.duration / phraseInfo.duration);
            eventRenderer_->addEvent(copy);
        }
    }
}