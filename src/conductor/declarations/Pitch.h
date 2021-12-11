#ifndef DECLARATION_PITCH_H
#define DECLARATION_PITCH_H

#include "ADeclaration.h"

namespace documentModel
{
    namespace conductor
    {
        /// <declaration name="pitch">
        ///     Changes the pitch of an event.
        /// ## example: 
        /// ```
        /// onBeat(1) {
        ///   pitch = 60;
        /// }
        /// onBeat(3) {
        ///   pitch += 25%;
        /// }        
        /// ```
        /// </declaration>
        /// <value name="absolute" type="0..127"></value>
        /// <value name="percent"  type="0%..100%">percent base value is events pitch</value>
        class PitchDecl : public ADeclaration
        {
        public:
            virtual void perform(const Events &events) const override;
            virtual ~PitchDecl() = default;
        };
    }
}

#endif