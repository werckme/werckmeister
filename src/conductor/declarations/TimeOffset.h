#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// <declaration name="timeOffset">
    /// Changes the time position of an event.
    /// **Since this is an offset value, the set operation(=) has the same effect as the add(+=) or substract(-=) operation.**
    /// #### example:
    /// ```
    /// onBeat(1) {
    ///   timeOffset += 0.125; -- 1/8 offset
    /// }
    /// onBeat(3) {
    ///   timeOffset -= 25%;
    /// }
    /// ```
    /// </declaration>
    /// <value name="absolute" type="0..N"></value>
    /// <value name="percent"  type="0%..N%">percent base value is the events length</value>
    class TimeOffset : public ADeclaration
    {
    public:
        virtual void perform(const Events &events) const override;
        virtual ~TimeOffset() = default;
        virtual bool canApply(const com::midi::Event& event) override;
    };
}
