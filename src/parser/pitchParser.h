#include <boost/spirit/include/qi.hpp>
#include <documentModel/PitchDef.h>
#include <documentModel/AliasPitchDef.h>
#include <com/common.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/core/value.hpp>
#include <boost/phoenix/operator/arithmetic.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    documentModel::PitchDef,
    (documentModel::PitchDef::Pitch, pitch)(documentModel::PitchDef::Octave, octave))

BOOST_FUSION_ADAPT_STRUCT(
    documentModel::AliasPitchDef,
    (com::String, alias))

namespace parser
{
    struct PitchParser
    {
        typedef com::String::const_iterator Iterator;
        PitchParser();
        boost::spirit::qi::rule<Iterator, PitchDef(), boost::spirit::ascii::space_type> pitchOrAlias_;
        boost::spirit::qi::rule<Iterator, AliasPitchDef(), boost::spirit::ascii::space_type> extendedPitch_;
        boost::spirit::qi::rule<Iterator, AliasPitchDef(), boost::spirit::ascii::space_type> alias_;
        boost::spirit::qi::rule<Iterator, PitchDef(), boost::spirit::ascii::space_type> pitch_;
    };
}