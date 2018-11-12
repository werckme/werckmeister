#include "Document.h"
#include <algorithm>

namespace sheet {
	IStyleDefServer::ConstStyleValueType Document::getStyle(const fm::String &name, const fm::String &sectionName) const
	{
		StyleDefs::const_iterator it;
		if (name == FM_STRING("?")) {
			it = styleDefs.begin();
		}
		else {
			it = styleDefs.find(name);
		}
		if (it == styleDefs.end()) {
			return nullptr;
		}
		StyleDef::Sections::const_iterator section;
		const auto& sectionContainer = it->second.sections;
		if (sectionName == FM_STRING("?"))
		{
			section = sectionContainer.begin();
		}
		else {
			section = std::find_if(sectionContainer.begin(), sectionContainer.end(), [sectionName](const auto&x) { return x.name == sectionName; });
		}
		if (section == sectionContainer.end()) {
			return nullptr;
		}
		return &(section->tracks);

	}
	IStyleDefServer::ConstChordValueType Document::getChord(const fm::String &name) const
	{
		ChordDefs::const_iterator it;
		if (name == FM_STRING("?")) {
			it = chordDefs.begin();
		}
		else {
			it = chordDefs.find(name);
		}
		if (it == chordDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}
}