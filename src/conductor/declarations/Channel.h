#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// <declaration name="midiChannel">
    ///  changes the channel of the selected event.
    /// #### example:
    /// ```\n
    /// onBeat(1) { \n
    ///   midiChannel = 1; \n
    /// } \n
    /// ```
    /// </declaration>
    /// <value name="midiChannel" type="0..15"></value>
    class SetChannel : public ADeclaration
    {
    public:
        SetChannel();
        virtual void perform(const Events &events) const override;
        virtual ~SetChannel() = default;
    private:
    };
}
