#ifndef ISELECTOR_H
#define ISELECTOR_H

#include <fm/IRegisterable.h>

namespace sheet
{
    namespace conductor
    {
        class ISelector : public fm::IRegisterable
        {
        public:
            virtual ~ISelector() = default;
        };
    }
}

#endif