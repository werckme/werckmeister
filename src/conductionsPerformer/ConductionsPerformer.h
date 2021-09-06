#ifndef CONDUCTIONS_PERFORMER_H
#define CONDUCTIONS_PERFORMER_H

#include "IConductionsPerformer.h"
#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <fm/midi.hpp>

namespace sheet {
	class ConductionsPerformer : public IConductionsPerformer {
	protected:
		fm::midi::MidiPtr          _midifile;
	public:
		ConductionsPerformer(fm::midi::MidiPtr midifile) : _midifile(midifile) {}
		virtual void applyConductions(DocumentPtr document) override;
	};
}

#endif