#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"
#include <com/exception.hpp>
#include <compiler/context/AContext.h>
#include <com/midi.hpp>

namespace sheet {
	std::pair<com::midi::MidiPtr, sheet::DocumentPtr> processFile(const std::string &file, sheet::Warnings &outWarnings, const com::midi::MidiConfig *midiConfig = nullptr);
	com::midi::MidiPtr processFile(sheet::DocumentPtr document, sheet::Warnings &outWarnings, const com::midi::MidiConfig *midiConfig = nullptr);
	sheet::DocumentPtr createDocument(const std::string &file);
	void onCompilerError(const com::Exception &ex);
	void onCompilerError(const std::exception &ex);
	void onCompilerError();
}

#endif