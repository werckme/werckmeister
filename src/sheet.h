#ifndef SHEET_HPP
#define SHEET_HPP
#include "forward.hpp"

namespace sheet {
	fm::midi::MidiPtr processFile(const std::string &file);
	fm::midi::MidiPtr processFile(sheet::DocumentPtr document);
	sheet::DocumentPtr createDocument(const std::string &file);
	sheet::DocumentPtr createDocumentByString(const fm::String &sheetText, const std::vector<fm::String> &usings);
}

#endif