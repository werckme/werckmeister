#ifndef MODIFICTAION_BEND_HPP
#define MODIFICTAION_BEND_HPP

#include "AModification.h"
#include <compiler/argumentNames.h>

#define FM_BEND_ALTERNAIVE_PARAM_VALUE_NOT_SET "-1"

namespace sheet {
    namespace compiler {
        class Bend : public AModification {
        public:
            Bend() = default;
            virtual ~Bend() = default;
            virtual void perform(IContextPtr ctx, Events &evs) override;
            double value = 0.5;
            enum BendMode { To, From };
            BendMode mode = To;
            fm::IHasParameter::ParametersByNames parameters = {
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.Bend.Mode, 	    0,  "to"),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.Bend.Value, 	    1,  "50"),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.Bend.BendTo,	    2,  FM_BEND_ALTERNAIVE_PARAM_VALUE_NOT_SET),
                FM_PARAMETER_DEFAULT_DEF	(argumentNames.Bend.BendFrom,   3,  FM_BEND_ALTERNAIVE_PARAM_VALUE_NOT_SET),
            };
            virtual ParametersByNames & getParameters() { return this->parameters; }
        private:
            void prepareValues();
        };
    }
}

#endif