#ifndef NAMED_ARGUMENTS_H
#define NAMED_ARGUMENTS_H

#include <string>

struct ArgumentNames {

    struct {
        std::string WithName                = "setName";
        std::string IsType                  = "isType";
        std::string UsePort                 = "usePort";
        std::string Offset                  = "withOffset";
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
        std::string Use                   = "use";
    } SetVoicingStrategy; 
    
    struct {
        std::string Range                   = "range";
    } SimpleGuitarVoicingStrategy; 
};

extern ArgumentNames argumentNames;  

#endif