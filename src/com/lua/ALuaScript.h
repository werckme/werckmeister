#pragma once


#include <com/common.hpp>
#include <string>

extern "C" {
    struct lua_State;
}

namespace documentModel {

    namespace lua {

        class ALuaScript {
        public:
            ALuaScript(const com::String &path);
            virtual ~ALuaScript();
            virtual bool hasFunction(const std::string &name) const;
            virtual bool canExecute() const = 0;
            virtual void assertCanExecute() const = 0;
            virtual void call(size_t numArgs, size_t numResult);
            virtual void addPackagePath(const com::String &path);
            const com::String & path() const { return _path; }
        protected:
            void error (const std::string &msg);
            lua_State *L = nullptr;
        private:
            const com::String &_path;
            void addSearchPaths();
        };
    }
}


