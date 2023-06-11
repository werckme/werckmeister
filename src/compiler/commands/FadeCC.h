#pragma once

#include "SetCC.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>
#include "SetFade.h"

namespace compiler
{
    /// <command name="fadeCC" where="voice">
    /// Fades a CC value from a start to an end value.
    /// #### examples
    /// `/fadeCC: _name="modulation" _from=10 _to=100 _curve="lin"/ -- fades a modulation value by controller name`
    /// `/fadeCC: _nr=1 _from=10 _to=100 _curve="lin"/ -- fades a modulation value by controller number`
    /// #### supported CC names *(if using name paramenter instead of cc number)*
    /// * BankSelectMSB
    /// * Modulation
    /// * BreathController
    /// * FootController
    /// * PortamentoTime
    /// * MainVolume 
    /// * Balance
    /// * Panorama
    /// * Expression
    /// * EffectControl1 
    /// * EffectControl2 
    /// * BankSelectLSB 
    /// * Hold1
    /// * Portamento
    /// * Sostenuto
    /// * SoftPedal
    /// * Legato
    /// * Hold2
    /// * PortamentoControl
    /// * Effects1Depth
    /// * Effects2Depth
    /// * Effects3Depth
    /// * Effects4Depth
    /// * Effects5Depth
    /// * AllSoundsOff
    /// * ControllerReset
    /// * LocalControl
    /// * AllNotesOff
    /// * OmniOff
    /// * OmniOn
    /// * MonoOn
    /// * MonoOff
    /// ![supported curve types](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/curve-types.png)
    /// </command>
    /// <param name="nr" type="0..N" position="0" optional="0" >The number of the controller</param>
    /// <param name="from" type="0..127" position="1" optional="0" >the start value</param>
    /// <param name="to" type="0..127" position="2" optional="0" >the end value</param>
    /// <param name="name"  type="text" position="3" optional="1" >a controller name, can be used instead of a number. (supported names, see list above)</param>
    /// <param name="curve" position="4" default="lin" optional="1" type="lin,quad,cub,quart,quint,exp">The fade curve type.</param>
    class FadeCC : public SetCC
    {
    public:
        typedef SetCC Base;
        com::IHasParameter::ParametersByNames parameters = {

            FM_PARAMETER_DEFAULT_DEF(argumentNames.FadeCC.Nr, 0, NoCCNumberValue),
            FM_PARAMETER_DEF(argumentNames.FadeCC.From, 1),
            FM_PARAMETER_DEF(argumentNames.FadeCC.To, 2),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.FadeCC.Duration, 3, NoNameValue),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.FadeCC.Curve, 4, SetFade::CurveTypeLin),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.FadeCC.Name, 5, NoNameValue),
        };
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr) override;
    };
}
