#pragma once

#include <documentModel/objects/Event.h>
#include <compiler/error.hpp>

namespace compiler
{
    class AUsingAnEvent
    {
    public:
        virtual ~AUsingAnEvent() = default;
        const Event &event() const
        {
            checkEvent();
            return *_event;
        }
        void event(const Event &event) { this->_event = &event; }

    private:
        void checkEvent() const
        {
            if (!_event)
            {
                FM_THROW(Exception, "related event == null");
            }
        }
        const Event *_event = nullptr;
    };
}
