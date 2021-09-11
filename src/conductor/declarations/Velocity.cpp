#include "Velocity.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Velocity::perform(fm::midi::Event* noteOn, fm::midi::Event*) const 
        {
            if (declaration.unit == ConductionRule::Declaration::UnitPercent) {
                FM_THROW(compiler::Exception, "not yet impl.");
            }
            auto value = declaration.value / 100 * 127;
            value = std::max(0.0, std::min(value, 127.0));
            noteOn->parameter2(fm::Byte(value));
        }
    }
}