#ifndef ADECLARATION_H
#define ADECLARATION_H

#include "IDeclaration.h"
#include <optional>

namespace sheet
{
    namespace conductor
    {
        class ADeclaration : public IDeclaration
        {
        public:
            virtual void perform(const Events &events) const = 0;
            virtual ~ADeclaration() = default;
            virtual void setDeclarationData(const ConductionRule::Declaration &val) override { declaration = val; }
            virtual const ConductionRule::Declaration &getDeclarationData() const override { return declaration; }
            // priority of a declaration higher values means higher priority
            virtual double specificity() const override;
            virtual void specificity(double val) override { _specificity = val; }
        protected:
            ConductionRule::Declaration declaration;
            typedef std::function<double()> FGetValue;
            typedef std::function<std::optional<double>()> FGetOptionalValue;
            typedef std::function<void(fm::midi::Event *, double)> FSetValue;
            void performImpl(fm::midi::Event *noteOn,
                             fm::midi::Event *noteOff,
                             double inputValue,
                             double min, double max,
                             const FGetValue &getOriginalValue,
                             const FGetValue &getPercentBaseValue,
                             const FGetOptionalValue &getPredecessorValue,
                             const FSetValue &setNoteOnValue,
                             const FSetValue &setNoteOffValue) const;
        private:
            double _specificity = 0;
        };
    }
}

#endif