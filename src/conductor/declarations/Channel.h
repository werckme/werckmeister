#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// <declaration name="cc-decl">
    ///  adds a cc event at the same position as the selected event.
    ///  &gt; the `%` unit and the `+=`, `-=`, `=&amp; +` operations are not supported.
    /// #### example:
    /// ```\n
    /// onBeat(1) { \n
    ///   bend = "Modulation(100)"; \n
    /// } \n
    /// ```
    /// ```\n
    /// onBeat(1) { \n
    ///   bend = "cc1(100)"; \n
    /// } \n
    /// ```
    /// </declaration>
    /// <value name="cc type and value" type="string">a contoller name followed by the target number enlcosed by parentheses. See https://werckme.github.io/manual#cc. Alternatively you can write cc0..127.</value>
    class SetChannel : public ADeclaration
    {
    public:
        SetChannel();
        virtual void perform(const Events &events) const override;
        virtual ~SetChannel() = default;
    private:
    };
}
