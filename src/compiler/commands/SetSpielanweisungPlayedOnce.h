#ifndef SETSPIELANWEISUNGPLAYEDONCE_HPP
#define SETSPIELANWEISUNGPLAYEDONCE_HPP

#include "SetSpielanweisung.h"


namespace sheet {
    namespace compiler {
        class SetSpielanweisungPlayedOnce : public SetSpielanweisung
        {
        public:
            virtual void execute(IContextPtr );
        };
    }
}

#endif