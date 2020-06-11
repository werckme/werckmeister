#include "SetVoicingStrategy.h"
#include <compiler/context/IContext.h>
#include <fm/werckmeister.hpp>

namespace sheet {
    namespace compiler {
        void SetVoicingStrategy::execute(IContext* context)
        {
            auto meta = context->voiceMetaData();
			bool isAlreadySet = meta->voicingStrategy && meta->voicingStrategy->name() == voicingStrategy->name();
            if (isAlreadySet) {
				return;
			}
			meta->voicingStrategy = this->voicingStrategy;
        }

        void SetVoicingStrategy::setArguments(const Arguments &args)
        {
            Base::setArguments(args);
            auto voicingStrategyName = parameters[argumentNames.SetVoicingStrategy.Use].value<fm::String>();
            this->voicingStrategy = fm::getWerckmeister().getVoicingStrategy(voicingStrategyName);
            if (args.size() <= 1) {
                return;
            }
            Arguments remaining (args.begin() + 1, args.end());
            this->voicingStrategy->setArguments(remaining);
        }
    }
}