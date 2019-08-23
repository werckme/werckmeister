#include "Bend.h"
#include "compiler/AContext.h"
#include <algorithm>
#include <fm/literals.hpp>
#include "sheet/tools.h"

namespace sheet {
    namespace compiler {

        void Bend::perform(AContext *ctx, Events &events)
        {
            using namespace fm;
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
            // swap result
        }

        void Bend::setArguments(const Event::Args &args)
        {
            value = getArgument<double>(args, 2) / 100.;
            auto modestr = getArgument<fm::String>(args, 1);
            if (modestr == FM_STRING("from")) {
                mode = From;
            } else {
                mode = To;
            }
        }

    }
}