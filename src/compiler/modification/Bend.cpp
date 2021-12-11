#include "Bend.h"
#include "compiler/context/IContext.h"
#include <algorithm>
#include <com/literals.hpp>
#include <com/tools.h>

namespace documentModel {
    namespace compiler {

        void Bend::perform(IContextPtr ctx, Events &events)
        {
            using namespace com;
            prepareValues();
            if (events.empty()) {
                return;
            }
            const auto &ev = events.front();
            if (ev.duration == 0) {
                return;
            }
            double b = 0.5;
            double e = value;
            if (mode==From) {
                b = value;
                e = 0.5;
            }
            double c = e - b;
            double d = static_cast<double>(ev.duration);
			for (double t=0; t<ev.duration; t+=static_cast<double>( 1.0_N64 )) {
                double x = c*t/d + b;
                Event pitchBendEvent;
                pitchBendEvent.type = Event::PitchBend;
                pitchBendEvent.pitchBendValue = x;
                pitchBendEvent.offset = t;
                events.push_back(pitchBendEvent);
            }
            Event finalResetPitchBend;
            finalResetPitchBend.type = Event::PitchBend;
            finalResetPitchBend.pitchBendValue = 0.5;
            finalResetPitchBend.offset = ev.duration + 1;
            events.push_back(finalResetPitchBend);
        }

        void Bend::prepareValues()
        {
            if (parameters[argumentNames.Bend.BendTo].strValue() != FM_BEND_ALTERNAIVE_PARAM_VALUE_NOT_SET) {
                value = parameters[argumentNames.Bend.BendTo].value<int>() / 100.;
                mode = To;
                return;
            }
            if (parameters[argumentNames.Bend.BendFrom].strValue() != FM_BEND_ALTERNAIVE_PARAM_VALUE_NOT_SET) {
                value = parameters[argumentNames.Bend.BendFrom].value<int>() / 100.;
                mode = From;
                return;
            }
            
            value = parameters[argumentNames.Bend.Value].value<int>() / 100.;
            auto modestr = parameters[argumentNames.Bend.Mode].value<com::String>();
            if (modestr == FM_STRING("from")) {
                mode = From;
            } else {
                mode = To;
            }
        }

    }
}