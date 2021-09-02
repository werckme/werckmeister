#include "DefineInstrumentSection.h"
#include <compiler/context/MidiContext.h>
#include <compiler/error.hpp>

namespace sheet {
    namespace compiler {
        void DefineInstrumentSection::execute(IContextPtr  context)
        {
            if (sectionInstruments.empty()) {
                FM_THROW(Exception, "missing instruments for this section");
            }
            auto midiContext = getMidiContext(context);
            auto name = parameters[argumentNames.InstrumentSection.WithName].value<fm::String>();
            midiContext->defineInstrumentSection(name, sectionInstruments);
        }

        void DefineInstrumentSection::setArguments(const Arguments& args)
        {
            Base::setArguments(args);
            bool containsWithName = false;
            for (const auto &arg : args) {
                if (arg.name == argumentNames.InstrumentSection.WithName) {
                    containsWithName = true;
                    continue;
                }
                sectionInstruments.push_back(arg.value);
            }
            if (!containsWithName && !sectionInstruments.empty()) {
                // in this case the first argument is the name, so remove it from the list
                sectionInstruments.pop_front();
            }
        }

    }
}