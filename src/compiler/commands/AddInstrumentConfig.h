#ifndef ADDINSTRUMENTCONFIG_HPP
#define ADDINSTRUMENTCONFIG_HPP

#include "ACommand.h"
#include <compiler/argumentNames.h>
#include <list>
#include <memory>


namespace sheet {
    namespace compiler {
        /// <command name="instrumentConf" where="document">
        /// With `instrumentConf` you are able to config a specific instrument.
        /// Following settings can be used:
        ///  * volume
        ///  * pan
        ///  * voicing strategy
        ///  * mod
        ///  * velocity remap
        /// ### examples
        /// #### mixed settings
        /// **positional:** 
        /// `instrumentConf: piano volume 100 pan 50;`
        /// **named:**
        /// `instrumentConf: _for=piano _set=volume _to=50 _set=pan _to=50;`
        /// #### setup a mod
        /// **positional:** 
        /// `instrumentConf: piano mod myLuaMod bar; --the bar argument belongs to "myLuaMod"`
        /// **named:**
        /// `instrumentConf: _for=piano _set=mod _use=myLuaMod _myLuaModFoo=bar;`
        /// #### setup a velocity remap
        /// With `remapVelocity` you are able to change the velocity values behind `ppppp..fffff`.
        /// In the example below the velcity for `p` will be set to 100 and the value for `f` will be set to 10.
        /// The value range is 0..100. (100=127 Midi velocity)
        /// **named:**
        /// `instrumentConf: _set=remapVelocity _p=100 _f=10;`
        /// **positional:**
        /// `remapVelocity` dosen't supports positional arguments
        /// </command>
        /// <param name="for" position="0" >The name of the target instrument. This is the only "unique" parameter for this command. All further parameters are specific to its related setting.
        /// </param>
        class AddInstrumentConfig : public ACommand
        {
        public:
            typedef ACommand Base;
            static const std::vector<fm::String> SupportedConfigCommands;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEF		    (argumentNames.InstrumentConf.ForInstrument, 	0)
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
            virtual void execute(IContextPtr );
            virtual void setArguments(const Arguments &args) override;
        private:
            typedef fm::String CommandName;
            typedef std::pair<CommandName, std::shared_ptr<ACommand>> ConfigCommand;
            typedef std::list<ConfigCommand> ConfigCommands;
            ConfigCommands _configCommands;
        };
        inline fm::String InstrumentConfigCommandName(const fm::String &cmdName) 
        {
            return "InstrumentConfigCommandPrefix_" + cmdName;
        }
    }
}

#endif