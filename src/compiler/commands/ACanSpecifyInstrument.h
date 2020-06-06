#ifndef SHEET_ACANSPECIFYINSTRUMENT_HPP
#define SHEET_ACANSPECIFYINSTRUMENT_HPP


#include "ICanSpecifyInstrument.h"

namespace sheet {
    namespace compiler {
        class ACanSpecifyInstrument : public ICanSpecifyInstrument
        {
        public:
            virtual ~ACanSpecifyInstrument() = default;
            virtual void setInstrument(AInstrumentDef *instrument) override { this->_instr = instrument; }
            virtual AInstrumentDef* getInstrument() override { return this->_instr; }
            virtual bool hasInstrument() const override { return this->_instr != nullptr; }
        private:
            AInstrumentDef *_instr = nullptr;
        };
    }
}

#endif