#ifndef FM_ALUAWITHPARAMETER_H
#define FM_ALUAWITHPARAMETER_H



extern "C" {
    struct lua_State;
    struct luaL_Reg;
}

#include <string>
#include <stdexcept>
#include <com/IHasParameter.h>

#define LUA_ALUA_WITH_PARAMETER_PARAMETERS "parameters"

namespace com {
    namespace lua {
        class ALuaWithParameter  {
        public:
            virtual IHasParameter::ParametersByNames & getParameters(lua_State *);
        protected: 
            virtual IHasParameter::ParametersByNames popParameters(lua_State *);
            virtual com::Parameter popParameter(lua_State *, int position);
            virtual void pushParameters(lua_State *, const IHasParameter::ParametersByNames&);
            IHasParameter::ParametersByNames parameters;
        };
    }
}

#endif