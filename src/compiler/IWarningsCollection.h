#ifndef IWARNINGSCOLLECTION_H
#define IWARNINGSCOLLECTION_H


#include "Warning.hpp"

namespace sheet {
	namespace compiler {
        class IWarningsCollection {
        private:
        public:
            virtual const Warnings & warnings() const = 0;
        };
    }
}
#endif