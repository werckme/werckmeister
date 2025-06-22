#pragma once

#include "ADeclaration.h"

namespace conductor
{
    /// <declaration name="fade-decl">
    ///  adding a fade of an cc, or pitchbend value at the position of the selected event.
    ///  &gt; the `%` unit and the `+=`, `-=`, `=&amp; +` operations are not supported.
    /// #### example:
    /// ```\n
    /// onBeat(1) { \n
    ///   fade = "from(0), to(100), type(cc2), curve(exp), offset(-1)"; \n
    /// } \n
    /// ```
    /// ```\n
    /// onBeat(1) { \n
    ///   bend = "cc1(100)"; \n
    /// } \n
    /// ```
    /// </declaration>
    /// <value name="fade options" type="string">a comma separated list of fade options. E.g. \"type(Modulation)\, from(0) ...", </value>
    /// <value name="type" type="string">could be either a controller (See https://werckme.github.io/manual#cc-declr) or type(bend)</value>
    /// <value name="from" type="0..N">the fade start value, in case of a bend the range is 0..100 where 50 is the neutral position. Otherwise it is 0..127</value>
    /// <value name="to" type="0..N">Optional. the fade end value, in case of a bend the range is 0..100 where 50 is the neutral position. Otherwise it is 0..127</value>
    /// <value name="duration" type="0..N">the fade duration in quarters</value>
    /// <value name="curve" type="0..N">Optional. the curve type see https://www.werckme.org/manual#fade</value>
    /// <value name="offset" type="-N..N">an optional position offset in quarters</value>
    class Fade : public ADeclaration
    {
    public:
        struct FadeOptions
        {
            enum { TypeBend = 0xFF };
            int type = 0;
            int from = 0;
            int to = 0;
            com::String curve = "lin";
            com::Ticks offset = 0;
            com::Ticks duration = 1;
        };
        Fade(com::midi::MidiPtr);
        virtual void perform(const Events &events) const override;
        virtual ~Fade() = default;
        void static parseFadeOptions(FadeOptions&, const com::String&);
    private:
        com::midi::MidiPtr midi;
    };
}
