#include "valueParser.h"
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/core/value.hpp>
#include <boost/phoenix/operator/arithmetic.hpp>
#include "parserSymbols.h"

namespace parser
{
    namespace
    {
        PitchSymbols pitchSymbols_;
        OctaveSymbols octaveSymbols_;
        DurationSymbols durationSymbols_;
    }

    const std::string ValueParser::ALLOWED_META_ARGUMENT = "a-zA-Z0-9.";

    ValueParser::ValueParser()
    {
        using namespace boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        using ascii::char_;
        using boost::phoenix::at_c;
        using boost::phoenix::push_back;
        using boost::spirit::qi::repeat;
        using qi::_1;
        using qi::_val;
        using qi::attr;
        using qi::lexeme;
        using qi::lit;
        using qi::int_;
        using namespace documentModel;
        extendedPitch_.name("extended pitch");
        extendedPitch_ %= "" >> char_("hijklmnopqsuvwxyz") >> repeat(0, 5)[lit("'")[push_back(at_c<0>(_val), '\'')]] >> repeat(0, 5)[lit(",")[push_back(at_c<0>(_val), ',')]];

        pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
        alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
        pitchOrAlias_ %= pitch_ | alias_ | extendedPitch_;
        fraction_ %= int_ >> "/" >> int_;
        quoted_string.name("quoted string");
        quoted_string %= lexeme['"' > +(char_ - '"') > '"'];
    }
}