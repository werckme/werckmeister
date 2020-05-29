#ifndef NAMED_ARGUMENTS_H
#define NAMED_ARGUMENTS_H

#include <string>

struct ArgumentNames {

    struct {
        std::string WithName                = "setName";
        std::string IsType                  = "isType";
        std::string UsePort                 = "usePort";
        // @positional
        std::string OffsetIndicator         = "use";
        // @positional
        std::string OffsetValue             = "value";
        // @named-alternative
        std::string WithOffset              = "withOffset";
    } Device;

    struct {
        std::string WithName                = "withName";
        std::string OnDevice                = "onDevice";
        std::string Ch                      = "ch";
        std::string Cc                      = "cc";
        std::string Pc                      = "pc";
    } InstrumentDef;

    struct {
        std::string ForInstrument           = "for";
    } InstrumentConf;

    struct {
        std::string SetTo                   = "to";
    } SetVolume;

    struct {
        std::string SetTo                   = "to";
    } SetPan;

    struct {
        std::string Use                     = "use";
    } SetVoicingStrategy; 
    
    struct {
        std::string Range                   = "range";
    } SimpleGuitarVoicingStrategy;

    struct {
        // @positional
        std::string Mode                    = "mode";
        // @positional
        std::string Value                   = "value";
        // @named-alternative
        std::string BendTo                  = "to";
        // @named-alternative
        std::string BendFrom                = "from";        
    } Bend; 

    struct {
        std::string Use                     = "use";
    } AddMod;

    struct {
        std::string CC                     = "cc";
        std::string PC                     = "pc";
    } SelectMidiSound;

    struct {
        std::string Value                  = "value";
    } SetExpression;

    struct {
        std::string Bpm                     = "bpm";
    } SetTempo;

    struct {
        std::string Upper                  = "upper";
        std::string Lower                  = "lower";
    } SetSignature;

    struct {
        std::string Use                  = "use";
    } SetInstrument;

};

extern ArgumentNames argumentNames;  

#endif