#ifndef NAMED_ARGUMENTS_H
#define NAMED_ARGUMENTS_H

#include <string>

struct ArgumentNames {
    struct {
        std::string WithName                = "withName";
        std::string IsType                  = "isType";
        std::string UsePort                 = "usePort";
        std::string Offset                  = "offset";
    } Device;
    struct {
        std::string WithName                = "withName";
        std::string OnDevice                = "onDevice";
        std::string Ch                      = "ch";
        std::string Cc                      = "cc";
        std::string Pc                      = "pc";
    } InstrumentDef;
};

extern ArgumentNames argumentNames;  

#endif