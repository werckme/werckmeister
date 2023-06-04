#pragma once

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <list>
#include <memory>

namespace compiler
{
    /// <command name="instrumentConf" where="document">
    /// With `instrumentConf` you are able to setup a specific instrument.
    /// Following settings can be applied:
    ///  * volume
    ///  * pan
    ///  * cc
    ///  * voicing strategy
    ///  * mod
    ///  * velocity remap
    /// #### examples
    /// ##### mixed settings
    /// `instrumentConf: piano volume 100 pan 50;`
    /// ##### setup a mod
    /// `instrumentConf: piano mod myLuaMod someValue; --the `someValue` argument belongs to "myLuaMod"`
    /// ##### setup a velocity remap
    /// With `remapVelocity` you are able to change the velocity values behind `ppppp..fffff` for a specific instrument.
    /// In the example below the velocity for `p` will be set to 100 and the value for `f` will be set to 10.
    /// The value range is 0..100. (100=127 Midi velocity)
    /// `instrumentConf: _set=remapVelocity _p=100 _f=10;`
    /// </command>
    /// <param name="for" position="0" >The name of the target instrument. This is the only "unique" parameter for this command. All further parameters are specific to its related setting.
    /// </param>
    class AddInstrumentConfig : public ACommand
    {
    public:
        typedef ACommand Base;
        static const std::vector<com::String> SupportedConfigCommands;
        com::IHasParameter::ParametersByNames parameters = {
            FM_PARAMETER_DEF(argumentNames.InstrumentConf.ForInstrument, 0)};
        virtual ParametersByNames &getParameters() { return this->parameters; }
        virtual void execute(IContextPtr);
        virtual void setArguments(const Arguments &args) override;

    private:
        typedef com::String CommandName;
        typedef std::pair<CommandName, std::shared_ptr<ACommand>> ConfigCommand;
        typedef std::list<ConfigCommand> ConfigCommands;
        ConfigCommands _configCommands;
    };
    inline com::String InstrumentConfigCommandName(const com::String &cmdName)
    {
        return "InstrumentConfigCommandPrefix_" + cmdName;
    }
}
