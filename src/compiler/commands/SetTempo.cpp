#include "SetTempo.h"
#include <compiler/context/AContext.h>
#include <compiler/metaCommands.h>
#include <iostream>

namespace sheet {
    namespace compiler {
        void SetTempo::execute(AContext* context)
        {
            auto tempoParameter         = parameters[argumentNames.SetTempo.Bpm];
            auto tempoStringValue = tempoParameter.strValue();
            std::cout << tempoStringValue << std::endl;	
            if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_HALF) {
                context->setTempo(context->masterTempo() * 0.5);
                return;
            }
            if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_DOUBLE) {
                context->setTempo(context->masterTempo() * 2);
                return;
            }
            if (tempoStringValue == SHEET_META__SET_TEMPO_VALUE_NORMAL) {
                context->setTempo(context->masterTempo());
                return;
            }			
            fm::BPM bpm = tempoParameter.value<fm::BPM>();	
            
            context->setTempo(bpm);
        }
    }
}