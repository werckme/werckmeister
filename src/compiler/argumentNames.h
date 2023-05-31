#pragma once

#include <string>

struct ArgumentNames
{

    struct
    {
        std::string WithName = "setName";
        std::string IsType = "isType";
        std::string UsePort = "usePort";
        std::string OffsetIndicator = "use";
        std::string OffsetValue = "value";
        std::string WithOffset = "withOffset";
        std::string UseFont = "useFont";
    } Device;

    struct
    {
        std::string WithName = "setName";
        std::string OnDevice = "onDevice";
        std::string Ch = "ch";
        std::string BankMsb = "bankMsb";
        std::string BankLsb = "bankLsb";
        std::string Pc = "pc";
        std::string Cc = "cc";
    } InstrumentDef;

    struct
    {
        std::string WithName = "setName";
    } InstrumentSection;

    struct
    {
        std::string ForInstrument = "for";
    } InstrumentConf;

    struct
    {
        std::string SetTo = "to";
    } SetVolume;

    struct
    {
        std::string SetTo = "to";
    } SetPan;

    struct
    {
        std::string Use = "use";
    } SetVoicingStrategy;

    struct
    {
        std::string Range = "range";
    } SimpleGuitarVoicingStrategy;

    struct
    {
        std::string Mode = "mode";
        std::string Value = "value";
        std::string BendTo = "to";
        std::string BendFrom = "from";
    } Bend;

    struct
    {
        std::string Use = "use";
    } AddMod;

    struct
    {
        std::string CC = "cc";
        std::string BankLsb = "bankLsb";
        std::string BankMsb = "bankMsb";
        std::string PC = "pc";
    } SelectMidiSound;

    struct
    {
        std::string Value = "value";
    } SetExpression;

    struct
    {
        std::string Bpm = "bpm";
    } SetTempo;

    struct
    {
        std::string Upper = "upper";
        std::string Lower = "lower";
    } SetSignature;

    struct
    {
        std::string Use = "use";
    } SetInstrument;

    struct
    {
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

    struct
    {
        std::string Name = "name";
    } Mark;

    struct
    {
        std::string Ignore = "ignore";
        std::string Repeat = "repeat";
        std::string To = "to";
    } Jump;

    struct
    {
        std::string FadeTo = "to";
        std::string FadeFrom = "from";
        std::string FadeDuration = "duration";
        std::string Curve = "curve";
    } SetFade;

    struct
    {
        std::string Text = "text";
    } Cue;

    struct
    {
        std::string Template = "template";
        std::string Replace = "replace";
    } Fill;
    struct
    {
        std::string Name = "name";
    } SetChord;
    struct
    {
        std::string Nr = "nr";
        std::string Name = "name";
        std::string Value = "value";
    } SetCC;
    struct
    {
        std::string Nr = "nr";
        std::string Name = "name";
        std::string From = "from";
        std::string To = "to";
        std::string Duration = "duration";
        std::string Curve = "curve";
    } FadeCC;
};

extern ArgumentNames argumentNames;
