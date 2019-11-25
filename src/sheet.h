#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"
#include <fm/exception.hpp>
#include <compiler/AContext.h>

namespace sheet {
	fm::midi::MidiPtr processFile(const std::string &file, sheet::Warnings &outWarnings);
	fm::midi::MidiPtr processFile(sheet::DocumentPtr document, sheet::Warnings &outWarnings);
	sheet::DocumentPtr createDocument(const std::string &file);
	void onCompilerError(const fm::Exception &ex);
	void onCompilerError(const std::exception &ex);
	void onCompilerError();
}

#endif