#include "Arpeggio.h"
#include "compiler/context/AContext.h"
#include <algorithm>
#include <fm/tools.h>

namespace sheet {
    namespace compiler {

        void Arpeggio::perform(AContext *ctx, Events &events)
        {
			Events result;
			if (events.empty()) {
				return;
			}
			const auto& ev = events.front();
			const auto& pitches = ev.pitches;
			auto duration = ev.duration;
			if (pitches.empty()) {
				return;
			}
			std::vector<PitchDef> pitchesCopy(pitches.begin(), pitches.end());

			std::sort(pitchesCopy.begin(), pitchesCopy.end(), [this](const auto &a, const auto &b) { 
				if (direction == Up) {
					return a < b;
				}
				return a > b;
			});

			auto arpduration = duration / pitches.size();
			int noteNumber = 0;
			for (const auto &pitch : pitchesCopy)
			{
				Event arpNote = ev;
				arpNote.pitches.clear();
				arpNote.pitches.insert(pitch);
				arpNote.duration = arpduration;
				arpNote.offset = arpduration * static_cast<fm::Ticks>(noteNumber++);
				result.push_back(arpNote);
			}
			events.swap(result);
        }

    }
}