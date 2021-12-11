#ifndef SHEET_AMIDICONTEXTCOMMAND_HPP
#define SHEET_AMIDICONTEXTCOMMAND_HPP


#include "ACommand.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>

namespace documentModel {
    namespace compiler {
        class AMidiContextCommand : public ACommand
        {
        public:
            virtual MidiContext * getMidiContext(IContextPtr context) const 
            {
                auto midiContext = dynamic_cast<MidiContext*>(context.get());
                if (midiContext == nullptr) {
                    FM_THROW(Exception, "failed to get midi context");
                }
                return midiContext;
            }
        };
    }
}

#endif