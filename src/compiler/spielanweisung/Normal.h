#ifndef SPIELANWEISUNG_NORMAL_HPP
#define SPIELANWEISUNG_NORMAL_HPP

#include "ASpielanweisung.h"

namespace sheet {
    namespace compiler {
        class Normal : public ASpielanweisung {
        public:
            Normal() = default;
            virtual ~Normal() = default;
            virtual void perform(AContext *ctx, const Event &ev) override;
        };
    }
}

#endif