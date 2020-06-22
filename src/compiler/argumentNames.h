#ifndef NAMED_ARGUMENTS_H
#define NAMED_ARGUMENTS_H

#include <string>

struct ArgumentNames {

    struct CDevice {
        std::string WithName                = "setName";
        std::string IsType                  = "isType";
        std::string UsePort                 = "usePort";
        // @positional
        std::string OffsetIndicator         = "use";
        // @positional
        std::string OffsetValue             = "value";
        // @named @alternative
        std::string WithOffset              = "withOffset";
    } Device;

    struct CInstrumentDef {
        std::string WithName                = "setName";
        std::string OnDevice                = "onDevice";
        std::string Ch                      = "ch";
        std::string Cc                      = "cc";
        std::string Pc                      = "pc";
    } InstrumentDef;

    struct CInstrumentConf {
        std::string ForInstrument           = "for";
    } InstrumentConf;

    struct CSetVolume {
        std::string SetTo                   = "to";
    } SetVolume;

    struct CSetPan {
        std::string SetTo                   = "to";
    } SetPan;

    struct CSetVoicingStrategy {
        std::string Use                     = "use";
    } SetVoicingStrategy; 
    
    struct CSimpleGuitarVoicingStrategy {
        std::string Range                   = "range";
    } SimpleGuitarVoicingStrategy;

    struct CBend {
        // @positional
        std::string Mode                    = "mode";
        // @positional
        std::string Value                   = "value";
        // @named @alternative
        std::string BendTo                  = "to";
        // @named @alternative
        std::string BendFrom                = "from";        
    } Bend; 

    struct CAddMod {
        std::string Use                     = "use";
    } AddMod;

    struct CSelectMidiSound {
        std::string CC                     = "cc";
        std::string PC                     = "pc";
    } SelectMidiSound;

    struct CSetExpression {
        std::string Value                  = "value";
    } SetExpression;

    struct CSetTempo {
        std::string Bpm                     = "bpm";
    } SetTempo;

    struct CSetSignature {
        std::string Upper                  = "upper";
        std::string Lower                  = "lower";
    } SetSignature;

    struct CSetInstrument {
        std::string Use                  = "use";
    } SetInstrument;

    struct CVelocityRemap {
        std::string PPPPP = "ppppp";
        std::string PPPP = "pppp";
        std::string PPP = "ppp";
        std::string PP = "pp";
        std::string P = "p";
        std::string F = "f";
        std::string FF = "ff";
        std::string FFF = "fff";
        std::string FFFF = "ffff";
        std::string FFFFF = "fffff";
    } VelocityRemap;

};

extern ArgumentNames argumentNames;  

#endif