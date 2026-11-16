#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace compiler
{
    /// <command name="modval" where="voice">
    /// Changes an argument value of a mod.
    /// #### examples
    /// `/modval: arpeggio _direction="down"/`
    /// <param name="mod" position="0" type="text">The name of the modification to adjust. This is the only "unique" parameter for this command. All further parameters are specific to its related modification.</param>
    class ModVal : public ACommand
    {
    public:
        typedef ACommand Base;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.ModVal.Mod, 0)};
        virtual ParametersByNames &getParameters() override { return this->parameters; }
        virtual void execute(IContextPtr) override;
        virtual void setArguments(const Arguments &args) override;
        void update(AModificationPtr theMod, const com::String &parameterName, const com::String &stringValue);
    protected:
        Arguments modArgs;
    };
}
