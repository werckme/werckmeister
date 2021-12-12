#pragma once

#include <documentModel/objects/Event.h>
#include <compiler/error.hpp>

namespace compiler
{
    class AUsingAnEvent
    {
    public:
        virtual ~AUsingAnEvent() = default;
        const documentModel::Event &event() const
        {
            checkEvent();
            return *_event;
        }
        void event(const documentModel::Event &event) { this->_event = &event; }

    private:
        void checkEvent() const
        {
            if (!_event)
            {
                FM_THROW(Exception, "related event == null");
            }
        }
        const documentModel::Event *_event = nullptr;
    };
}
