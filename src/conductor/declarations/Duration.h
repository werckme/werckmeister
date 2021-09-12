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
        /// atBeat(1) {
        ///   duration += 0.125; -- 1/8 longer
        /// }
        /// atBeat(3) {
        ///   duration -= 25%;
        /// }        
        /// ```
        /// </selector>
        /// <value name="absolute" type="0..N"></value>
        /// <value name="percent"  type="0..N%">percent base value is the events length</value>
        class Duration : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~Duration() = default;
        };
    }
}

#endif