#ifndef ISELECTOR_H
#define ISELECTOR_H

#include <fm/IRegisterable.h>
#include <fm/midi.hpp>
#include <sheet/objects/ConductionSheetDef.h>

namespace sheet
{
    namespace conductor
    {
        class ISelector : public fm::IRegisterable
        {
        public:
            virtual bool isMatch(const ConductionSelector::Arguments&, const fm::midi::Event&) const = 0;
            virtual ~ISelector() = default;
        };
    }
}

#endif