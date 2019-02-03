#ifndef COMPILER_STYLERENDERER_H
#define COMPILER_STYLERENDERER_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include "sheet/ChordDef.h"
#include "sheet/StyleDefServer.h"
#include "compiler/voicings/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>
#include "forward.hpp"
#include "metaData.h"
#include "AContext.h"

namespace sheet {
    namespace compiler {
        class StyleRenderer {
        public:
            void render(AContextPtr ctx, fm::Ticks duration);
        };
    }
}

#endif