#include "ALuaScript.h"
#include <lua.hpp>
#include <exception>
#include <sstream>
#include <boost/filesystem.hpp>
#include <fm/werckmeister.hpp>
namespace sheet {
    
    namespace lua {

        ALuaScript::ALuaScript(const fm::String &path) : _path(path)
        {
            L= luaL_newstate();
            luaL_openlibs(L);
            auto dir = boost::filesystem::path(path).parent_path().generic_string();
            addPackagePath(dir);
            addSearchPaths();
            if (luaL_dofile(L, path.c_str())) {
                error(std::string(lua_tostring(L, -1)));
            }
        }

        void ALuaScript::addSearchPaths()
        {
            auto paths = fm::getWerckmeister().searchPaths();
            for (const auto &path : paths) {
                auto genericPath = boost::filesystem::path(path).generic_string();
                addPackagePath(genericPath);
            }
        }

        void ALuaScript::addPackagePath(const fm::String &path)
        {
            std::string pathcommand("package.path = package.path .. ';");
            pathcommand += path + "/?.lua'";
            luaL_dostring(L, pathcommand.c_str());
        }

        ALuaScript::~ALuaScript()
        {
            lua_close(L);
        }

        bool ALuaScript::hasFunction(const std::string &name) const 
        {
            lua_getglobal(L, name.c_str());
            if (lua_isfunction(L, -1) == 1) {
                return true;
            }
            return false;
        }

        void ALuaScript::error(const std::string &msg)
        {
            std::stringstream ss;
            ss << _path << ": ";
            ss << msg;
            throw std::runtime_error(ss.str());
        }

        void ALuaScript::call(std::size_t numArgs, std::size_t numResult)
        {
            if (lua_pcall(L, numArgs, numResult, 0)) {
                error(std::string(lua_tostring(L, -1)));
            }
        }
    }
}