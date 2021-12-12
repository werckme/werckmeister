#include "Arpeggio.h"
#include "compiler/context/IContext.h"
#include <algorithm>
#include <com/tools.h>

namespace compiler
{

	void Arpeggio::perform(IContextPtr ctx, Events &events)
	{
		Events result;
		if (events.empty())
		{
			return;
		}
		const auto &ev = events.front();
		const auto &pitches = ev.pitches;
		auto duration = ev.duration;
		if (pitches.empty())
		{
			return;
		}
		std::vector<documentModel::PitchDef> pitchesCopy(pitches.begin(), pitches.end());

		std::sort(pitchesCopy.begin(), pitchesCopy.end(), [this](const auto &a, const auto &b)
				  {
					  if (direction == Up)
					  {
						  return a < b;
					  }
					  return a > b;
				  });

		auto arpduration = duration / pitches.size();
		int noteNumber = 0;
		for (const auto &pitch : pitchesCopy)
		{
			documentModel::Event arpNote = ev;
			arpNote.pitches.clear();
			arpNote.pitches.push_back(pitch);
			arpNote.duration = arpduration;
			arpNote.offset = arpduration * static_cast<com::Ticks>(noteNumber++);
			result.push_back(arpNote);
		}
		events.swap(result);
	}

}