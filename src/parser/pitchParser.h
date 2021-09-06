#include <boost/spirit/include/qi.hpp>
#include <sheet/AliasPitchDef.h>
#include <fm/common.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/core/value.hpp>
#include <boost/phoenix/operator/arithmetic.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	sheet::PitchDef,
	(sheet::PitchDef::Pitch, pitch)
	(sheet::PitchDef::Octave, octave)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::AliasPitchDef,
	(fm::String, alias)
)


namespace sheet {
    namespace compiler {
        struct PitchParser {
            typedef fm::String::const_iterator Iterator;
            PitchParser();
            boost::spirit::qi::rule<Iterator, PitchDef(), boost::spirit::ascii::space_type> pitchOrAlias_;
            boost::spirit::qi::rule<Iterator, AliasPitchDef(), boost::spirit::ascii::space_type> extendedPitch_;
            boost::spirit::qi::rule<Iterator, AliasPitchDef(), boost::spirit::ascii::space_type> alias_;
            boost::spirit::qi::rule<Iterator, PitchDef(), boost::spirit::ascii::space_type> pitch_;
        };
    }
}