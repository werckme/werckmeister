#ifndef SETSPIELANWEISUNG_HPP
#define SETSPIELANWEISUNG_HPP

#include "AddMod.h"
#include <compiler/argumentNames.h>
#include <forward.hpp>

namespace sheet {
    namespace compiler {
        class SetSpielanweisung : public AddMod
        {
        public:
            virtual void execute(IContextPtr ) override;
        };
    }
}

#endif