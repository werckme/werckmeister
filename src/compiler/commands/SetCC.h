#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>
#include "AUsingAnEvent.h"
#include <compiler/ASheetEventRenderer.h>

namespace compiler
{
    /// <command name="cc" where="voice">
    /// Adds a midi CC message.
    /// #### examples
    /// `/cc: _nr=1 _value=10/ --modulation by controller number`
    /// `/cc: _name="modulation" _value=10/ --modulation value by controller name`
    /// #### supported CC names
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
    /// </command>
    /// <param name="nr" type="0..N" position="0" optional="1" >The number of the controller</param>
    /// <param name="value" type="0..127" position="1" optional="0" >the controller values</param>
    /// <param name="name"  type="[BankSelectMSB,Modulation,BreathController,FootController,PortamentoTime,MainVolume,Balance,Panorama,Expression,EffectControl1,EffectControl2,BankSelectLSB,Hold1,Portamento,Sostenuto,SoftPedal,Legato,Hold2,PortamentoControl,Effects1Depth,Effects2Depth,Effects3Depth,Effects4Depth,Effects5Depth,AllSoundsOff,ControllerReset,LocalControl,AllNotesOff,OmniOff,OmniOn,MonoOn,MonoOff]" position="2" optional="1" >a controller name, can be used instead of a number. (supported names, see list below)</param>
    class SetCC : public ACommand, public AUsingAnEvent
    {
    public:
        typedef std::unordered_map<com::String, int>  ControllerNumberMap;
        SetCC(ASheetEventRendererPtr eventRenderer) : eventRenderer(eventRenderer) {}
        typedef ACommand Base;
        static com::String NoNameValue; 
        static int NoCCNumberValue; 
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEFAULT_DEF(argumentNames.SetCC.Nr, 0, NoCCNumberValue),
            FM_PARAMETER_DEF(argumentNames.SetCC.Value, 1),
            FM_PARAMETER_DEFAULT_DEF(argumentNames.SetCC.Name, 2, NoNameValue)
        };
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr) override;
        int findControllerNr(const com::String &name) const;
        int getControllerNr();
        void renderController(IContextPtr context, int controllerNumber, int controllerValue, com::Ticks timeOffset = 0);
        static const ControllerNumberMap& getControllerNumberMap();
    private:
        ASheetEventRendererPtr eventRenderer;
    };
}
