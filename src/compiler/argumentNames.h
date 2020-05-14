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
};

extern ArgumentNames argumentNames;  

#endif