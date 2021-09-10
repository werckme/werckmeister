#include "Velocity.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(fm::midi::Event& ev) const 
        {
            if (declaration.unit == ConductionRule::Declaration::UnitPercent) {
                FM_THROW(compiler::Exception, "not yet impl.");
            }
            auto value = declaration.value / 100 * 127;
            value = std::max(0.0, std::min(value, 127.0));
            ev.parameter2(fm::Byte(value));
        }
    }
}