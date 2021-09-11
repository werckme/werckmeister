#include "TimeOffset.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void TimeOffset::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOffOrNull) const 
        {
            if (declaration.unit == ConductionRule::Declaration::UnitPercent) {
                FM_THROW(compiler::Exception, "not yet impl.");
            }
            auto value = declaration.value * fm::PPQ;
            noteOn->absPosition(noteOn->absPosition() + value);
            if (!noteOffOrNull) {
                return;
            }
            noteOffOrNull->absPosition(noteOffOrNull->absPosition() + value);
        }
    }
}