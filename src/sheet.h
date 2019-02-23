#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"
#include <fm/exception.hpp>

namespace sheet {
	fm::midi::MidiPtr processFile(const std::string &file);
	fm::midi::MidiPtr processFile(sheet::DocumentPtr document);
	sheet::DocumentPtr createDocument(const std::string &file);
	void onCompilerError(const fm::Exception &ex);
	void onCompilerError(const std::exception &ex);
	void onCompilerError();
}

#endif