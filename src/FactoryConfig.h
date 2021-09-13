#include <fm/werckmeister.hpp>
#include <type_traits>

namespace sheet 
{
    template<class TInjector>
    struct FactoryConfig 
    {
        FactoryConfig(TInjector &injector) : injector_(injector) 
        {
        }
        FactoryConfig(const FactoryConfig&&) = delete;
		FactoryConfig& operator=(const FactoryConfig&&) = delete;
        void init()
        {
            initVoicingStrategies();
            initModifications();
            initCommands();
            initConductor();
        }
        void initVoicingStrategies();
        void initModifications();
        void initCommands();
        void initConductor();
    private:
        TInjector &injector_;
        template<class TRegisterable>
        void register_(const fm::String &name)
        {
            static_assert(std::is_convertible<TRegisterable*, fm::IRegisterable*>::value, "TRegisterable must implement IRegisterable");
            fm::getWerckmeister().register_<TRegisterable>(name, []() 
            { 
                return std::make_shared<TRegisterable>(); 
            });
        }
        
    };
}


// Voicing Stategies
#include <compiler/voicingStrategies.h>
#include <compiler/voicings/DirectVoicingStrategy.h>
#include <compiler/voicings/SimpleGuitar.h>

namespace sheet
{
    template<class TInjector>
    void FactoryConfig<TInjector>::initVoicingStrategies()
    {
        // _FM_Register(DirectVoicingStrategy, SHEET_VOICING_STRATEGY_AS_NOTATED);
        // _FM_Register(SimpleGuitar, SHEET_VOICING_STRATEGY_SIMPLE_GUITAR);
        // _FM_Register(DirectVoicingStrategy, SHEET_VOICING_STRATEGY_DEFAULT);
    }
}




// Modifications
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/spielanweisung/Arpeggio.h>
#include <compiler/spielanweisung/Vorschlag.h>
#include <compiler/modification/modifications.h>
#include <compiler/modification/Bend.h>

namespace sheet
{
    template<class TInjector>
    void FactoryConfig<TInjector>::initModifications()
    {
        using namespace compiler;
        // _FM_Register(Arpeggio, SHEET_SPIELANWEISUNG_ARPEGGIO);
        // _FM_Register(Vorschlag, SHEET_SPIELANWEISUNG_VORSCHLAG);
        // _FM_Register(Bend, SHEET_MOD_BEND);
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
#include <compiler/commands/SetFade.h>
#include <compiler/commands/NopCommand.h>
#include <compiler/commands/Cue.h>
#include <compiler/commands/Fill.h>
#include <compiler/commands/DefineInstrumentSection.h>

namespace sheet
{
    template<class TInjector>
    void FactoryConfig<TInjector>::initCommands()
    {
        using namespace compiler;
        // _FM_Register(DefineMidiInstrument, SHEET_META__MIDI_INSTRUMENT_DEF);
        // _FM_Register(SetVolume, SHEET_META__SET_VOLUME);
        // _FM_Register(InstrumentConfigSetVolume, InstrumentConfigCommandName(SHEET_META__SET_VOLUME));
        // _FM_Register(SetPan, SHEET_META__SET_PAN);
        // _FM_Register(InstrumentConfigSetPan, InstrumentConfigCommandName(SHEET_META__SET_PAN));
        // _FM_Register(AddInstrumentConfig, SHEET_META__SET_INSTRUMENT_CONFIG);
        // _FM_Register(SetVoicingStrategy, SHEET_META__SET_VOICING_STRATEGY);
        // _FM_Register(InstrumentConfigSetVoicingStrategy, InstrumentConfigCommandName(SHEET_META__SET_VOICING_STRATEGY));
        // _FM_Register(AddMod, SHEET_META__SET_MOD);
        // _FM_Register(AddModPlayedOnce, SHEET_META__SET_MOD_ONCE);
        // _FM_Register(AddVorschlag, SHEET_META__SET_VORSCHLAG);
        // _FM_Register(SelectMidiSound, SHEET_META__MIDI_SOUNDSELECT);
        // _FM_Register(SetExpression, SHEET_META__SET_EXPRESSION);
        // _FM_Register(SetExpressionPlayedOnce, SHEET_META__SET_EXPRESSION_PLAYED_ONCE);
        // _FM_Register(SetTempo, SHEET_META__SET_TEMPO);
        // _FM_Register(SetSpielanweisung, SHEET_META__SET_SPIELANWEISUNG);
        // _FM_Register(SetSpielanweisungPlayedOnce, SHEET_META__SET_SPIELANWEISUNG_ONCE);
        // _FM_Register(SetSignature, SHEET_META__SET_SIGNATURE);
        // _FM_Register(AddDevice, SHEET_META__SET_DEVICE);
        // _FM_Register(SetInstrument, SHEET_META__INSTRUMENT);
        // _FM_Register(InstrumentConfigAddMod, InstrumentConfigCommandName(SHEET_META__SET_MOD));
        // _FM_Register(InstrumentConfigVelocityRemap, InstrumentConfigCommandName(SHEET_META__VELOCITY_REMAP));
        // _FM_Register(Mark, SHEET_META__MARK);
        // _FM_Register(Jump, SHEET_META__JUMP);
        // _FM_Register(SetFade, SHEET_META__SET_FADE);
        // _FM_Register(NopCommand, SHEET_META__TRACK_META_KEY_TYPE);
        // _FM_Register(NopCommand, SHEET_META__INSTRUMENT);
        // _FM_Register(NopCommand, SHEET_META__TRACK_META_KEY_NAME);
        // _FM_Register(NopCommand, SHEET_META__SET_SHEET_TEMPLATE);
        // _FM_Register(NopCommand, SHEET_META__SHEET_TEMPLATE_POSITION);
        // _FM_Register(Fill, SHEET_META__SET_FILL_TEMPLATE);
        // _FM_Register(Cue, SHEET_META__ADD_CUE);
        // _FM_Register(DefineInstrumentSection, SHEET_META__DEFINE_INSTRUMENT_SECTION);
    }    
}

// Conductor
#include <conductor/conductorNames.h>
#include <conductor/selectors/AtBeat.h>
#include <conductor/selectors/FromPosition.h>
#include <conductor/selectors/ToPosition.h>
#include <conductor/selectors/Pitch.h>
#include <conductor/selectors/FromPitch.h>
#include <conductor/selectors/ToPitch.h>
#include <conductor/selectors/Instrument.h>
#include <conductor/declarations/Velocity.h>
#include <conductor/declarations/TimeOffset.h>
#include <conductor/declarations/Duration.h>

namespace sheet
{
    template<class TInjector>
    void FactoryConfig<TInjector>::initConductor()
    {
        using namespace conductor;
        fm::String namespace_ = "conductor.";
        // _FM_Register(AtBeat,  namespace_ + SHEET_CONDUCTOR_SEL__AT_BEAT);
        // _FM_Register(FromPosition, namespace_ + SHEET_CONDUCTOR_SEL__FROM_POSITION);
        // _FM_Register(ToPosition, namespace_ + SHEET_CONDUCTOR_SEL__TO_POSITION);
        // _FM_Register(Velocity, namespace_ + SHEET_CONDUCTOR_DEC__VELOCITY);
        // _FM_Register(TimeOffset, namespace_ + SHEET_CONDUCTOR_DEC__TIME_OFFSET);
        // _FM_Register(Duration, namespace_ + SHEET_CONDUCTOR_DEC__LENGTH);
        // _FM_Register(Pitch, namespace_ + SHEET_CONDUCTOR_SEL__PITCH);
        // _FM_Register(FromPitch, namespace_ + SHEET_CONDUCTOR_SEL__FROM_PITCH);
        // _FM_Register(ToPitch, namespace_ + SHEET_CONDUCTOR_SEL__TO_PITCH);
        // _FM_Register(Instrument, namespace_ + SHEET_CONDUCTOR_SEL__INSTRUMENT);
    }
}