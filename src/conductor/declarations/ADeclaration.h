#ifndef ADECLARATION_H
#define ADECLARATION_H

#include "IDeclaration.h"

namespace sheet
{
    namespace conductor
    {
        class ADeclaration : public IDeclaration
        {
        public:
            virtual void perform(fm::midi::Event* noteOn, fm::midi::Event* noteOff) const = 0;
            virtual ~ADeclaration() = default;
        protected:
            typedef std::function<double()> FGetValue;
            typedef std::function<void(fm::midi::Event*, double)> FSetValue;
            void performImpl(fm::midi::Event* noteOn, fm::midi::Event* noteOff, double inputValue, double min, double max, const FGetValue &getOriginalValue, const FGetValue &getPercentBaseValue, const FSetValue &setNoteOnValue, const FSetValue &setNoteOffValue) const;
        };
    }
}

#endif