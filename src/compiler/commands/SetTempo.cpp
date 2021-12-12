#include "SetTempo.h"
#include <compiler/context/IContext.h>
#include <compiler/metaCommands.h>

namespace compiler
{
    void SetTempo::execute(IContextPtr context)
    {
        auto tempoParameter = parameters[argumentNames.SetTempo.Bpm];
        auto tempoStringValue = tempoParameter.strValue();
        if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_HALF)
        {
            context->setTempo(context->masterTempo() * 0.5);
            return;
        }
        if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_DOUBLE)
        {
            context->setTempo(context->masterTempo() * 2);
            return;
        }
        if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_NORMAL)
        {
            context->setTempo(context->masterTempo());
            return;
        }

        com::BPM bpm = tempoParameter.value<com::BPM>();
        context->setTempo(bpm);
    }
}