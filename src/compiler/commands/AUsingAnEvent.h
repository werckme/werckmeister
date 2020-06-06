#ifndef SHEET_AUSINGANEVENT_HPP
#define SHEET_AUSINGANEVENT_HPP

#include <sheet/objects/Event.h>
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        class AUsingAnEvent
        {
        public:
            virtual ~AUsingAnEvent() = default;
            const Event & event() const { checkEvent(); return *_event; }
            void event(const Event &event) { this->_event = &event; }
        private:
            void checkEvent() const 
            {  
                if (!_event) {
                    FM_THROW(Exception, "related event == null");
                }
            }
            const Event *_event = nullptr;
        };
    }
}

#endif