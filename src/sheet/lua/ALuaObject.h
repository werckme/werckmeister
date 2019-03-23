#ifndef ALUAOBJECT_H
#define ALUAOBJECT_H

#define LUA_OBJECT_REF_TABLE_KEY "__luaobjectref"

extern "C" {
    struct lua_State;
    struct luaL_Reg;
}

#include <string>
#include <stdexcept>

namespace sheet {

    namespace lua {

        class ALuaObject {
        public:
            typedef std::string RefId;
            ALuaObject() = default;
            ALuaObject(const ALuaObject&) = delete;
            ALuaObject & operator=(const ALuaObject&) = delete;
            virtual ~ALuaObject() = default;
            RefId refId() const;
            virtual void push(lua_State *L, const luaL_Reg *functions, size_t numFunctions);
            template<class TObject>
            static TObject * getObject(const RefId &refId);
            template<class TObject>
            static TObject * getObject(lua_State *L, int idx);            
        private:
            static RefId _getRefIdFromObjectTable(lua_State *L, int idx);
        };

        ///////////////////////////////////////////////////////////////////////
        template<class TObject>
        TObject * ALuaObject::getObject(const RefId &refId)
        {
            ALuaObject *obj = reinterpret_cast<ALuaObject*>(std::stoll(refId));
            if (obj == nullptr) {
                throw std::runtime_error("invalid cast for ALuaObject");
            }
            return dynamic_cast<TObject*>(obj);
        }

        template<class TObject>
        TObject * ALuaObject::getObject(lua_State *L, int idx)
        {
            auto refId = _getRefIdFromObjectTable(L, idx);
            return getObject<TObject>(refId);
        }
    }
}


#endif