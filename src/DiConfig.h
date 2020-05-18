#include <fm/werckmeister.hpp>
#include <type_traits>


#define _FM_Register(type, name) static_assert(std::is_convertible<type*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable"); \
fm::getWerckmeister().register_<type>(name, [](){ return std::make_shared<type>(); })


// Voicing Stategies
#include <compiler/voicingStrategies.h>
#include <compiler/voicings/DirectVoicingStrategy.h>
#include <compiler/voicings/SimpleGuitar.h>

namespace sheet {
    namespace {
        const bool voicingStrategiesRegistered = ([]() {
            _FM_Register(DirectVoicingStrategy, SHEET_VOICING_STRATEGY_AS_NOTATED);
            _FM_Register(SimpleGuitar,          SHEET_VOICING_STRATEGY_SIMPLE_GUITAR);
            _FM_Register(DirectVoicingStrategy, SHEET_VOICING_STRATEGY_DEFAULT);
            return true;
        })();
    }
}

// Modifications
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/spielanweisung/Arpeggio.h>
#include <compiler/spielanweisung/Vorschlag.h>
#include <compiler/modification/modifications.h>
#include <compiler/modification/Bend.h>

namespace sheet {
    namespace compiler {
        namespace {
            const bool modificationsRegistered = ([]() {
                _FM_Register(Arpeggio,           SHEET_SPIELANWEISUNG_ARPEGGIO);
                _FM_Register(Vorschlag,          SHEET_SPIELANWEISUNG_VORSCHLAG);
                _FM_Register(Bend,               SHEET_MOD_BEND);
                return true;
            })();
        }
    }
}

// Commands
#include <compiler/metaCommands.h>
#include <compiler/commands/DefineMidiInstrument.h>
#include <compiler/commands/SetVolume.h>
#include <compiler/commands/SetPan.h>
namespace sheet {
    namespace compiler {
        namespace {
            const bool commandsRegistered = ([]() {
                _FM_Register(DefineMidiInstrument,     SHEET_META__MIDI_INSTRUMENT_DEF);
                _FM_Register(SetVolume,                SHEET_META__SET_VOLUME);
                _FM_Register(SetPan,                   SHEET_META__SET_PAN);
                return true;
            })();
        }
    }
}