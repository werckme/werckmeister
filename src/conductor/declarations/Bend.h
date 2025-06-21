#pragma once

#include "ADeclaration.h"
#include <forward.hpp>

namespace conductor
{
    /// <declaration name="bend">
    ///  adds a pitch bend event at the same position as the selected event.
    ///  &gt; the `%` unit and the `+=`, `-=`, `=&amp; +` operations are not supported.
    /// #### example:
    /// ```\n
    /// onBeat(1) { \n
    ///   bend = 60; \n
    /// } \n
    /// ```
    /// </declaration>
    /// <value name="absolute" type="0..100">50 is the neutral position of the bend</value>
    class Bend : public ADeclaration
    {
    public:
        Bend(com::midi::MidiPtr);
        virtual void perform(const Events &events) const override;
        virtual ~Bend() = default;
    private:
        com::midi::MidiPtr midi;
    };
}
