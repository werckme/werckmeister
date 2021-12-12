#include <com/werckmeister.hpp>
#include <memory>

template <class TInjector>
class FactoryConfig
{
public:
    FactoryConfig(TInjector &injector) : injector_(injector)
    {
    }
    FactoryConfig(const FactoryConfig &&) = delete;
    FactoryConfig &operator=(const FactoryConfig &&) = delete;
    void init()
    {
        initVoicingStrategies();
        initModifications();
        initCommands();
        initConductor();
    }

private:
    void initVoicingStrategies();
    void initModifications();
    void initCommands();
    void initConductor();
    TInjector &injector_;
    template <class TRegisterable>
    std::shared_ptr<TRegisterable> create()
    {
        // injector needs to create uniqe_ptr not shared_ptr otherwise the creation
        // is considered as singleton
        typedef std::unique_ptr<TRegisterable> PtrType;
        auto uniquePtr = injector_.template create<PtrType>();
        return std::shared_ptr<TRegisterable>(uniquePtr.release());
    }
    template <class TRegisterable>
    void register_(const com::String &name)
    {
        com::getWerckmeister().register_<TRegisterable>(name, [this]()
                                                        { return create<TRegisterable>(); });
    }
};

// Voicing Stategies
#include <compiler/voicingStrategies.h>
#include <compiler/voicings/DirectVoicingStrategy.h>
#include <compiler/voicings/SimpleGuitar.h>

template <class TInjector>
void FactoryConfig<TInjector>::initVoicingStrategies()
{
    register_<DirectVoicingStrategy>(SHEET_VOICING_STRATEGY_AS_NOTATED);
    register_<SimpleGuitar>(SHEET_VOICING_STRATEGY_SIMPLE_GUITAR);
    register_<DirectVoicingStrategy>(SHEET_VOICING_STRATEGY_DEFAULT);
}

// Modifications
#include <compiler/spielanweisung/spielanweisungen.h>
#include <compiler/spielanweisung/Arpeggio.h>
#include <compiler/spielanweisung/Vorschlag.h>
#include <compiler/modification/modifications.h>
#include <compiler/modification/Bend.h>

template <class TInjector>
void FactoryConfig<TInjector>::initModifications()
{
    using namespace compiler;
    register_<Arpeggio>(SHEET_SPIELANWEISUNG_ARPEGGIO);
    register_<Vorschlag>(SHEET_SPIELANWEISUNG_VORSCHLAG);
    register_<Bend>(SHEET_MOD_BEND);
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

template <class TInjector>
void FactoryConfig<TInjector>::initCommands()
{
    using namespace compiler;
    register_<DefineMidiInstrument>(SHEET_META__MIDI_INSTRUMENT_DEF);
    register_<SetVolume>(SHEET_META__SET_VOLUME);
    register_<InstrumentConfigSetVolume>(InstrumentConfigCommandName(SHEET_META__SET_VOLUME));
    register_<SetPan>(SHEET_META__SET_PAN);
    register_<InstrumentConfigSetPan>(InstrumentConfigCommandName(SHEET_META__SET_PAN));
    register_<AddInstrumentConfig>(SHEET_META__SET_INSTRUMENT_CONFIG);
    register_<SetVoicingStrategy>(SHEET_META__SET_VOICING_STRATEGY);
    register_<InstrumentConfigSetVoicingStrategy>(InstrumentConfigCommandName(SHEET_META__SET_VOICING_STRATEGY));
    register_<AddMod>(SHEET_META__SET_MOD);
    register_<AddModPlayedOnce>(SHEET_META__SET_MOD_ONCE);
    register_<AddVorschlag>(SHEET_META__SET_VORSCHLAG);
    register_<SelectMidiSound>(SHEET_META__MIDI_SOUNDSELECT);
    register_<SetExpression>(SHEET_META__SET_EXPRESSION);
    register_<SetExpressionPlayedOnce>(SHEET_META__SET_EXPRESSION_PLAYED_ONCE);
    register_<SetTempo>(SHEET_META__SET_TEMPO);
    register_<SetSpielanweisung>(SHEET_META__SET_SPIELANWEISUNG);
    register_<SetSpielanweisungPlayedOnce>(SHEET_META__SET_SPIELANWEISUNG_ONCE);
    register_<SetSignature>(SHEET_META__SET_SIGNATURE);
    register_<AddDevice>(SHEET_META__SET_DEVICE);
    register_<SetInstrument>(SHEET_META__INSTRUMENT);
    register_<InstrumentConfigAddMod>(InstrumentConfigCommandName(SHEET_META__SET_MOD));
    register_<InstrumentConfigVelocityRemap>(InstrumentConfigCommandName(SHEET_META__VELOCITY_REMAP));
    register_<Mark>(SHEET_META__MARK);
    register_<Jump>(SHEET_META__JUMP);
    register_<SetFade>(SHEET_META__SET_FADE);
    register_<NopCommand>(SHEET_META__TRACK_META_KEY_TYPE);
    register_<NopCommand>(SHEET_META__INSTRUMENT);
    register_<NopCommand>(SHEET_META__TRACK_META_KEY_NAME);
    register_<NopCommand>(SHEET_META__SET_SHEET_TEMPLATE);
    register_<NopCommand>(SHEET_META__SHEET_TEMPLATE_POSITION);
    register_<Fill>(SHEET_META__SET_FILL_TEMPLATE);
    register_<Cue>(SHEET_META__ADD_CUE);
    register_<DefineInstrumentSection>(SHEET_META__DEFINE_INSTRUMENT_SECTION);
}

// Conductor
#include <conductor/conductorNames.h>
#include <conductor/selectors/OnBeat.h>
#include <conductor/selectors/NotOnBeat.h>
#include <conductor/selectors/FromPosition.h>
#include <conductor/selectors/ToPosition.h>
#include <conductor/selectors/Pitch.h>
#include <conductor/selectors/FromPitch.h>
#include <conductor/selectors/ToPitch.h>
#include <conductor/selectors/Instrument.h>
#include <conductor/selectors/FromBar.h>
#include <conductor/selectors/ToBar.h>
#include <conductor/selectors/FromBeat.h>
#include <conductor/selectors/ToBeat.h>
#include <conductor/selectors/OnBar.h>
#include <conductor/selectors/NthBar.h>
#include <conductor/selectors/Channel.h>
#include <conductor/selectors/Expression.h>
#include <conductor/declarations/Velocity.h>
#include <conductor/declarations/TimeOffset.h>
#include <conductor/declarations/Duration.h>
#include <conductor/declarations/Pitch.h>

template <class TInjector>
void FactoryConfig<TInjector>::initConductor()
{
    using namespace conductor;
    com::String selNamespace_ = "conductor.sel.";
    com::String declNamespace_ = "conductor.decl.";
    register_<OnBeat>(selNamespace_ + SHEET_CONDUCTOR_SEL__ON_BEAT);
    register_<NotOnBeat>(selNamespace_ + SHEET_CONDUCTOR_SEL__NOT_ON_BEAT);
    register_<FromPosition>(selNamespace_ + SHEET_CONDUCTOR_SEL__FROM_POSITION);
    register_<ToPosition>(selNamespace_ + SHEET_CONDUCTOR_SEL__TO_POSITION);
    register_<Pitch>(selNamespace_ + SHEET_CONDUCTOR_SEL__PITCH);
    register_<FromPitch>(selNamespace_ + SHEET_CONDUCTOR_SEL__FROM_PITCH);
    register_<ToPitch>(selNamespace_ + SHEET_CONDUCTOR_SEL__TO_PITCH);
    register_<Instrument>(selNamespace_ + SHEET_CONDUCTOR_SEL__INSTRUMENT);
    register_<Channel>(selNamespace_ + SHEET_CONDUCTOR_SEL__CHANNEL);
    register_<FromBar>(selNamespace_ + SHEET_CONDUCTOR_SEL__FROM_BAR);
    register_<ToBar>(selNamespace_ + SHEET_CONDUCTOR_SEL__TO_BAR);
    register_<FromBeat>(selNamespace_ + SHEET_CONDUCTOR_SEL__FROM_BEAT);
    register_<ToBeat>(selNamespace_ + SHEET_CONDUCTOR_SEL__TO_BEAT);
    register_<OnBar>(selNamespace_ + SHEET_CONDUCTOR_SEL__ON_BAR);
    register_<NthBar>(selNamespace_ + SHEET_CONDUCTOR_SEL__NTH_BAR);
    register_<Expression>(selNamespace_ + SHEET_CONDUCTOR_SEL__EXPRESSION);
    register_<Velocity>(declNamespace_ + SHEET_CONDUCTOR_DEC__VELOCITY);
    register_<TimeOffset>(declNamespace_ + SHEET_CONDUCTOR_DEC__TIME_OFFSET);
    register_<Duration>(declNamespace_ + SHEET_CONDUCTOR_DEC__LENGTH);
    register_<PitchDecl>(declNamespace_ + SHEET_CONDUCTOR_DEC__PITCH);
}