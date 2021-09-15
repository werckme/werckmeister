#ifndef DECLARATION_PITCH_H
#define DECLARATION_PITCH_H

#include "ADeclaration.h"

namespace sheet
{
    namespace conductor
    {
        /// <declaration name="pitch">
        ///     Changes the pitch of an event.
        /// ## example: 
        /// ```
        /// atBeat(1) {
        ///   pitch = 60;
        /// }
        /// atBeat(3) {
        ///   pitch += 25%;
        /// }        
        /// ```
        /// </selector>
        /// <value name="absolute" type="0..127"></value>
        /// <value name="percent"  type="0%..100%">percent base value is events pitch</value>
        class PitchDecl : public ADeclaration
        {
        public:
            virtual void perform(fm::midi::Event*, fm::midi::Event*) const override;
            virtual ~PitchDecl() = default;
        };
    }
}

#endif