#include "SetCC.h"
#include <compiler/context/IContext.h>
#include <com/werckmeister.hpp>
#include <algorithm>
#include <unordered_map>
#include <compiler/error.hpp>
#include <compiler/ICompilerVisitor.h>

namespace compiler
{
    namespace 
    {
        std::unordered_map<com::String, int> _controllerNames(
        {
            { "BankSelectMSB", 0x0 },
            { "Modulation", 0x1 },
            { "BreathController", 0x2 },
            { "FootController", 0x4 },
            { "PortamentoTime", 0x5 },
            { "MainVolume", 0x7 },
            { "Balance", 0x8 },
            { "Panorama", 0xA },
            { "Expression", 0xB },
            { "EffectControl1", 0xC },
            { "EffectControl2", 0xD },
            { "BankSelectLSB", 0x20 },
            { "Hold1", 0x40 },
            { "Portamento", 0x41 },
            { "Sostenuto", 0x42 },
            { "SoftPedal", 0x43 },
            { "Legato", 0x44 },
            { "Hold2", 0x45 },
            { "PortamentoControl", 0x54 },
            { "Effects1Depth", 0x5B },
            { "Effects2Depth", 0x5C },
            { "Effects3Depth", 0x5D },
            { "Effects4Depth", 0x5E },
            { "Effects5Depth", 0x5F },
            { "AllSoundsOff", 0x78 },
            { "ControllerReset", 0x79 },
            { "LocalControl", 0x7A },
            { "AllNotesOff", 0x7B },
            { "OmniOff", 0x7C },
            { "OmniOn", 0x7D },
            { "MonoOn", 0x7E },
            { "MonoOff", 0x7F },
        });
    }
    com::String SetCC::NoNameValue = "NoName";
    int SetCC::NoCCNumberValue = -1;
    int SetCC::getControllerNr()
    {
        auto &_parameters = getParameters();
        auto nr = _parameters[argumentNames.SetCC.Nr].value<int>();
        auto name = _parameters[argumentNames.SetCC.Name].value<com::String>();
        if (nr == NoCCNumberValue && name == NoNameValue)
        {
            FM_THROW(Exception, "neither cc number nor a name was given.");
        }
        if (name != NoNameValue)
        {
            nr = findControllerNr(name);
        }
        return nr;
    }
    void SetCC::execute(IContextPtr context)
    {
        int nr = getControllerNr();
        auto value = parameters[argumentNames.SetCC.Value].value<int>();
        context->setContinuousController(nr, value);
    }
    int SetCC::findControllerNr(const com::String &name) const
    {
        const auto &it = _controllerNames.find(name);
        if (it == _controllerNames.end())
        {
            FM_THROW(Exception, "invalid cc name: '" + name + "'. Please find here a complete list of supported cc names https://werckme.github.io/manual#cc");
        }
        return it->second;
    }
}