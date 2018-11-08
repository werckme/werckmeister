#include "Document.h"

namespace sheet {
	const StyleDef * Document::getStyle(const fm::String &name) const
	{
		auto it = styleDefs.find(name);
		if (it == styleDefs.end()) {
			return nullptr;
		}
		return &it->second;
	}
	const ChordDef::Intervals * Document::getChord(const fm::String &name) const
	{
		auto it = chordDefs.find(name);
		if (it == chordDefs.end()) {
			return nullptr;
		}
		return &it->second;
	}
}