#include "SetVoicingStrategy.h"
#include <compiler/context/AContext.h>
#include <fm/werckmeister.hpp>

namespace sheet {
    namespace compiler {
        void SetVoicingStrategy::execute(AContext* context)
        {
            if (this->hasInstrument()) {
                getInstrument()->voicingStrategy = this->_voicingStrategy;
                return;
            }
            auto meta = context->voiceMetaData();
			bool isAlreadySet = meta->voicingStrategy && meta->voicingStrategy->name() == _voicingStrategy->name();
            if (isAlreadySet) {
				return;
			}
			meta->voicingStrategy = this->_voicingStrategy;
        }

        void SetVoicingStrategy::setArguments(const Arguments &args)
        {
            Base::setArguments(args);
            auto voicingStrategyName = parameters[argumentNames.SetVoicingStrategy.Use].value<fm::String>();
            this->_voicingStrategy = fm::getWerckmeister().getVoicingStrategy(voicingStrategyName);
            if (args.size() <= 1) {
                return;
            }
            Arguments remaining (args.begin() + 1, args.end());
            this->_voicingStrategy->setArguments(remaining);
        }
    }
}