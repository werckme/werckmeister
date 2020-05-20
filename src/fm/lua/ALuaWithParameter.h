#ifndef FM_ALUAWITHPARAMETER_H
#define FM_ALUAWITHPARAMETER_H



extern "C" {
    struct lua_State;
    struct luaL_Reg;
}

#include <string>
#include <stdexcept>
#include <fm/IHasParameter.h>

#define LUA_ALUA_WITH_PARAMETER_PARAMETERS "parameters"

namespace fm {
    namespace lua {
        class ALuaWithParameter  {
        public:
            virtual IHasParameter::ParametersByNames & getParameters(lua_State *);
        protected: 
            virtual IHasParameter::ParametersByNames popParameters(lua_State *);
            virtual fm::Parameter popParameter(lua_State *, int position);
        private:
            IHasParameter::ParametersByNames _parameters;
        };
    }
}

#endif