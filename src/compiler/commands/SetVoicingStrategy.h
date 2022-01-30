#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <compiler/voicings/VoicingStrategy.h>

namespace compiler
{
    /// <command name="voicingStrategy" where="voice">
    /// Adds a modification to the track.
    /// ### example
    /// `/voicingStrategy: asNotated/`
    /// Voicing strategies can be external lua scripts, or one of these internal strategies:
    /// * asNotated
    /// * simpleGuitar
    /// </command>
    /// <param name="use" position="0" type="text">The name of the strategy to load. This is the only "unique" parameter for this command. All further parameters are specific to its related strategy.</param>

    class SetVoicingStrategy : public ACommand
    {
    public:
        typedef ACommand Base;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.SetVoicingStrategy.Use, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void setArguments(const Arguments &args) override;
        virtual void execute(IContextPtr);

    protected:
        VoicingStrategyPtr voicingStrategy = nullptr;
    };
}
