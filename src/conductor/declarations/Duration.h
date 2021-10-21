#ifndef DECLARATION_LENGTH_H
#define DECLARATION_LENGTH_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        /// <declaration name="duration">
        ///     Changes the duration of an event. 
        /// ## example: 
        /// ```
        /// onBeat(1) {
        ///   duration += 0.125; -- 1/8 longer
        /// }
        /// onBeat(3) {
        ///   duration -= 25%;
        /// }        
        /// ```
        /// </declaration>
        /// <value name="absolute" type="0..N"></value>
        /// <value name="percent"  type="0..N%">percent base value is the events length</value>
        class Duration : public ADeclaration
        {
        public:
            virtual void perform(const Events &events) const override;
            virtual ~Duration() = default;
        };
    }
}

#endif