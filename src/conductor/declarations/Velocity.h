#ifndef DECLARATION_VELOCITY_H
#define DECLARATION_VELOCITY_H

#include "ADeclaration.h"

namespace documentModel
{
    namespace conductor
    {
        /// <declaration name="velocity">
        ///     Changes the velocity of an event.
        /// ## example: 
        /// ```
        /// onBeat(1) {
        ///   velocity = 127;
        /// }
        /// onBeat(3) {
        ///   velocity += 25%;
        /// }        
        /// ```
        /// </declaration>
        /// <value name="absolute" type="0..127"></value>
        /// <value name="percent"  type="0%..100%">percent base value is events velocity</value>
        class Velocity : public ADeclaration
        {
        public:
            virtual void perform(const Events &events) const override;
            virtual ~Velocity() = default;
        };
    }
}

#endif