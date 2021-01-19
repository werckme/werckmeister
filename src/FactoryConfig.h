#include <fm/werckmeister.hpp>
#include <type_traits>


#define _FM_Register(type, name) static_assert(std::is_convertible<type*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable"); \
fm::getWerckmeister().register_<type>(name, [](){ return std::make_shared<type>(); })

#define _FM_RegisterSingleton(type, name) static_assert(std::is_convertible<type*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable"); \
fm::getWerckmeister().register_<type>(name, [](){ static auto singleton = std::make_shared<type>(); return singleton; })

#define _FM_ReplaceRegistration(type, name) static_assert(std::is_convertible<type*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable"); \
fm::getWerckmeister().replaceRegistration<type>(name, [](){ return std::make_shared<type>(); })

#define _FM_ReplaceRegistrationSingleton(type, name) static_assert(std::is_convertible<type*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable"); \
fm::getWerckmeister().replaceRegistration<type>(name, [](){ static auto singleton = std::make_shared<type>(); return singleton; })

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
#include <compiler/commands/InstrumentConfigSetVolume.h>
#include <compiler/commands/SetPan.h>
#include <compiler/commands/InstrumentConfigSetPan.h>
#include <compiler/commands/AddInstrumentConfig.h>
#include <compiler/commands/SetVoicingStrategy.h>
#include <compiler/commands/InstrumentConfigSetVoicingStrategy.h>
#include <compiler/commands/AddMod.h>
#include <compiler/commands/AddModPlayedOnce.h>
#include <compiler/commands/AddVorschlag.h>
#include <compiler/commands/SelectMidiSound.h>
#include <compiler/commands/SetExpression.h>
#include <compiler/commands/SetExpressionPlayedOnce.h>
#include <compiler/commands/SetTempo.h>
#include <compiler/commands/SetSpielanweisung.h>
#include <compiler/commands/SetSpielanweisungPlayedOnce.h>
#include <compiler/commands/SetSignature.h>
#include <compiler/commands/AddDevice.h>
#include <compiler/commands/SetInstrument.h>
#include <compiler/commands/InstrumentConfigAddMod.h>
#include <compiler/commands/InstrumentConfigVelocityRemap.h>
#include <compiler/commands/Mark.h>
#include <compiler/commands/Jump.h>

namespace sheet {
    namespace compiler {
        namespace {
            const bool commandsRegistered = ([]() {
                _FM_Register(DefineMidiInstrument,                      SHEET_META__MIDI_INSTRUMENT_DEF);
                _FM_Register(SetVolume,                                 SHEET_META__SET_VOLUME);
                _FM_Register(InstrumentConfigSetVolume,                 InstrumentConfigCommandName(SHEET_META__SET_VOLUME));
                _FM_Register(SetPan,                                    SHEET_META__SET_PAN);
                _FM_Register(InstrumentConfigSetPan,                    InstrumentConfigCommandName(SHEET_META__SET_PAN));
                _FM_Register(AddInstrumentConfig,                       SHEET_META__SET_INSTRUMENT_CONFIG);
                _FM_Register(SetVoicingStrategy,                        SHEET_META__SET_VOICING_STRATEGY);
                _FM_Register(InstrumentConfigSetVoicingStrategy,        InstrumentConfigCommandName(SHEET_META__SET_VOICING_STRATEGY));
                _FM_Register(AddMod,                                    SHEET_META__SET_MOD);
                _FM_Register(AddModPlayedOnce,                          SHEET_META__SET_MOD_ONCE);
                _FM_Register(AddVorschlag,                              SHEET_META__SET_VORSCHLAG);
                _FM_Register(SelectMidiSound,                           SHEET_META__MIDI_SOUNDSELECT);
                _FM_Register(SetExpression,                             SHEET_META__SET_EXPRESSION);
                _FM_Register(SetExpressionPlayedOnce,                   SHEET_META__SET_EXPRESSION_PLAYED_ONCE);
                _FM_Register(SetTempo,                                  SHEET_META__SET_TEMPO);
                _FM_Register(SetSpielanweisung,                         SHEET_META__SET_SPIELANWEISUNG);
                _FM_Register(SetSpielanweisungPlayedOnce,               SHEET_META__SET_SPIELANWEISUNG_ONCE);
                _FM_Register(SetSignature,                              SHEET_META__SET_SIGNATURE);
                _FM_Register(AddDevice,                                 SHEET_META__SET_DEVICE);
                _FM_Register(SetInstrument,                             SHEET_META__INSTRUMENT);
                _FM_Register(InstrumentConfigAddMod,                    InstrumentConfigCommandName(SHEET_META__SET_MOD));
                _FM_Register(InstrumentConfigVelocityRemap,             InstrumentConfigCommandName(SHEET_META__VELOCITY_REMAP));
                _FM_Register(Mark,                                      SHEET_META__MARK);
                _FM_Register(Jump,                                      SHEET_META__JUMP);
                return true;
            })();
        }
    }
}
