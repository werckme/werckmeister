#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"
#include <fm/exception.hpp>
#include <compiler/context/AContext.h>
#include <fm/midi.hpp>

namespace sheet {
	std::pair<fm::midi::MidiPtr, sheet::DocumentPtr> processFile(const std::string &file, sheet::Warnings &outWarnings, const fm::midi::MidiConfig *midiConfig = nullptr);
	fm::midi::MidiPtr processFile(sheet::DocumentPtr document, sheet::Warnings &outWarnings, const fm::midi::MidiConfig *midiConfig = nullptr);
	sheet::DocumentPtr createDocument(const std::string &file);
	void onCompilerError(const fm::Exception &ex);
	void onCompilerError(const std::exception &ex);
	void onCompilerError();
}

#endif