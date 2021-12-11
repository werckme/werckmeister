#pragma once


#include "Warning.hpp"
#include <memory>

namespace documentModel {
	namespace compiler {
        class IWarningsCollection {
        private:
        public:
            virtual const Warnings & warnings() const = 0;
        };
        typedef std::shared_ptr<IWarningsCollection> IWarningsCollectionPtr;
    }
}
