#ifndef SHEET_AMIDICONTEXTCOMMAND_HPP
#define SHEET_AMIDICONTEXTCOMMAND_HPP


#include "ACommand.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        class AMidiContextCommand : public ACommand
        {
        public:
            virtual MidiContext * getMidiContext(IContext *context) const 
            {
                auto midiContext = dynamic_cast<MidiContext*>(context);
                if (midiContext == nullptr) {
                    FM_THROW(Exception, "failed to get midi context");
                }
                return midiContext;
            }
        };
    }
}

#endif