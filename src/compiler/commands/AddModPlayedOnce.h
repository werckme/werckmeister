#ifndef ADDMODPLAYEDONCE_HPP
#define ADDMODPLAYEDONCE_HPP

#include "AddMod.h"
#include <compiler/argumentNames.h>

namespace sheet {
    namespace compiler {
        class AddModPlayedOnce : public AddMod
        {
        public:
            virtual void execute(AContext*) override;
        };
    }
}

#endif