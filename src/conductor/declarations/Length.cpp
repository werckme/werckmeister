#include "Length.h"
#include <compiler/error.hpp>

namespace sheet
{
    namespace conductor
    {
        void Length::perform(fm::midi::Event* noteOn, fm::midi::Event* noteOffOrNull) const 
        {
            if (declaration.unit == ConductionRule::Declaration::UnitPercent) {
                FM_THROW(compiler::Exception, "not yet impl.");
            }
            if (!noteOffOrNull) {
                return;
            }
            auto value = declaration.value * fm::PPQ;
            noteOffOrNull->absPosition(noteOffOrNull->absPosition() + value);
        }
    }
}