#ifndef FM_ALUA_SCRIPT_H
#define FM_ALUA_SCRIPT_H


#include <fm/common.hpp>
#include <string>

extern "C" {
    struct lua_State;
}

namespace sheet {

    namespace lua {

        class ALuaScript {
        public:
            ALuaScript(const fm::String &path);
            virtual ~ALuaScript();
            virtual bool hasFunction(const std::string &name) const;
            virtual bool canExecute() const = 0;
            virtual void assertCanExecute() const = 0;
            virtual void call(size_t numArgs, size_t numResult);
            virtual void addPackagePath(const fm::String &path);
        protected:
            void error (const std::string &msg);
            lua_State *L = nullptr;
        private:
            const fm::String &_path;
        };
    }
}

#endif
