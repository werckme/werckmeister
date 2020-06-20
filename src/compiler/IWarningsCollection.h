#ifndef IWARNINGSCOLLECTION_H
#define IWARNINGSCOLLECTION_H


#include "Warning.hpp"
#include <memory>

namespace sheet {
	namespace compiler {
        class IWarningsCollection {
        private:
        public:
            virtual const Warnings & warnings() const = 0;
        };
        typedef std::shared_ptr<IWarningsCollection> IWarningsCollectionPtr;
    }
}
#endif