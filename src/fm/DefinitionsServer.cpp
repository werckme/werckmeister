#include "DefinitionsServer.h"

#if 0
namespace fm {
    namespace {
        template<typename TContainer>
        auto _findByName(const fm::String &name, const TContainer &container)
        {
            typename TContainer::const_iterator it;
            if (name == FM_STRING("?")) {
                it = container.begin();
            } else {
                it = container.find(name);
            }
            return it;			
        }
    }
    sheet::SheetTemplate * DefinitionsServer::findSheetTemplate(const fm::String &sheetTemplateName)
	{
		SheetTemplates &sheetTemplates = *sheetTemplates_;
		SheetTemplates::iterator it = sheetTemplates.find(sheetTemplateName);
		if (it == sheetTemplates.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	void DefinitionsServer::createSheetTemplatesMap()
	{
		// TODO: #126
		// sheetTemplates_ = std::make_unique<SheetTemplates>();
		// SheetTemplates &sheetTemplates = *sheetTemplates_;
		// compiler::Preprocessor preprocessor;
		// for(auto &track : this->sheetDef.tracks) {
		// 	try {
		// 		fm::String type = fm::getFirstMetaArgumentWithKeyName(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
		// 		if (type != SHEET_META__TRACK_META_VALUE_TYPE_SHEET_TEMPLATE) {
		// 			continue;
		// 		}
		// 		fm::String sheetTemplateName = fm::getFirstMetaArgumentWithKeyName(SHEET_META__TRACK_META_KEY_NAME, track.trackConfigs).value;
		// 		if (sheetTemplateName.empty()) {
		// 			FM_THROW(compiler::Exception, "missing 'name' for sheetTemplate track");
		// 		}					
		// 		auto sheetTemplate = findSheetTemplate(sheetTemplateName);
		// 		if (sheetTemplate == nullptr) {
		// 			sheetTemplates[sheetTemplateName] = SheetTemplate(sheetTemplateName);
		// 			sheetTemplate = &(sheetTemplates[sheetTemplateName]);
		// 		}
		// 		preprocessor.process(track);
		// 		sheetTemplate->tracks.push_back(&track);
		// 	} catch(const fm::Exception &ex) {
		// 		ex << compiler::ex_sheet_source_info(track);
		// 		throw;
		// 	}
		// }
	}

	DefinitionsServer::SheetTemplates & DefinitionsServer::sheetTemplates()
	{
		if (!sheetTemplates_) {
			createSheetTemplatesMap();
		}
		return *sheetTemplates_;
	}

	sheet::SheetTemplate DefinitionsServer::getSheetTemplate(const fm::String &name)
	{
		const SheetTemplates &sheetTemplates = this->sheetTemplates();
		// find sheetTemplate by name
		SheetTemplates::const_iterator it = _findByName(name, sheetTemplates);
		if (it == sheetTemplates.end()) {
			return SheetTemplateType();
		}
		return it->second;
	}

	IDefinitionsServer::ConstChordValueType DefinitionsServer::getChord(const fm::String &name)
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

	IDefinitionsServer::ConstPitchDefValueType DefinitionsServer::getAlias(fm::String alias)
	{
		PitchmapDefs::const_iterator it;
		it = pitchmapDefs.find(alias);
		
		if (it == pitchmapDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	PitchDef DefinitionsServer::resolvePitch(const PitchDef &pitch) const
	{
		if (pitch.alias.empty()) {
			return pitch;
		}
		const PitchDef *result = sheetTemplateDefServer()->getAlias(pitch.alias);
		if (result == nullptr) {
			FM_THROW(Exception, "could not resolve alias: " + pitch.alias);
		}
		return *result;
	}
}
#endif