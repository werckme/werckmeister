#ifndef SHEET_ICANSPECIFYINSTRUMENT_HPP
#define SHEET_ICANSPECIFYINSTRUMENT_HPP

#include <compiler/instrument.h>

namespace sheet {
    namespace compiler {
        class ICanSpecifyInstrument
        {
        public:
            virtual ~ICanSpecifyInstrument() = default;
            virtual void setInstrument(AInstrumentDef *instrument) = 0;
            virtual AInstrumentDef* getInstrument() = 0;
            virtual bool hasInstrument() const = 0;
        };
    }
}

#endif