#ifndef DECLARATION_VELOCITY_H
#define DECLARATION_VELOCITY_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        /// <declaration name="velocity">
        ///     Changes the velocity of an event.
        /// ## example: 
        /// ```
        /// atBeat(1) {
        ///   velocity = 127;
        /// }
        /// atBeat(3) {
        ///   velocity += 25%;
        /// }        
        /// ```
        /// </selector>
        /// <value name="absolute" type="0..127"></value>
        /// <value name="percent"  type="0%..100%">percent base value is events velocity</value>
        class Velocity : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~Velocity() = default;
        };
    }
}

#endif