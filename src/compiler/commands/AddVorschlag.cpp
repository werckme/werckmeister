#include "AddVorschlag.h"
#include <compiler/context/AContext.h>
#include <fm/werckmeister.hpp>
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/spielanweisung/Vorschlag.h>


namespace sheet {
    namespace compiler {
        void AddVorschlag::execute(AContext* context)
        {
            auto &wm = fm::getWerckmeister();
			auto meta = context->voiceMetaData();
			meta->spielanweisungOnce = wm.getSpielanweisung(SHEET_SPIELANWEISUNG_VORSCHLAG);
			auto vorschlag = std::dynamic_pointer_cast<Vorschlag>(meta->spielanweisungOnce);
			vorschlag->vorschlagNote = AUsingAnEvent::event();
        }
    }
}