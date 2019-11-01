#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"
#include <fm/exception.hpp>

namespace sheet {
	fm::midi::MidiPtr processFile(const std::string &file, bool stdoutWarnings = true);
	fm::midi::MidiPtr processFile(sheet::DocumentPtr document, bool showWarnings = true);
	sheet::DocumentPtr createDocument(const std::string &file);
	void onCompilerError(const fm::Exception &ex);
	void onCompilerError(const std::exception &ex);
	void onCompilerError();
}

#endif