#include "Velocity.h"

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(fm::midi::Event& ev) const 
        {
            ev.parameter2(127);
        }
    }
}