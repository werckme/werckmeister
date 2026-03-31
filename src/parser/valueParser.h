#include <boost/spirit/include/qi.hpp>
#include <documentModel/PitchDef.h>
#include <documentModel/AliasPitchDef.h>
#include <documentModel/objects/FractionValue.hpp>
#include <com/common.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/object.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/core/value.hpp>
#include <boost/phoenix/operator/arithmetic.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    documentModel::PitchDef,
    (documentModel::PitchDef::Pitch, pitch)
    (documentModel::PitchDef::Octave, octave)
    (bool, forceDegree)
)

BOOST_FUSION_ADAPT_STRUCT(
    documentModel::AliasPitchDef,
    (com::String, alias))

BOOST_FUSION_ADAPT_STRUCT(
    documentModel::FractionValue,
    (int, nominator)
    (int, denominator))

namespace parser
{
    namespace ascii = boost::spirit::ascii;
    enum PitchDefFields
    {
        PitchDefFieldPitch,
        PitchDefFieldOctace,
        PitchDefFieldForceDegree,
    };

    struct ValueParser
    {
        typedef com::String::const_iterator Iterator;
        ValueParser();
        boost::spirit::qi::rule<Iterator, documentModel::PitchDef(), boost::spirit::ascii::space_type> pitchOrAlias_;
        boost::spirit::qi::rule<Iterator, documentModel::AliasPitchDef(), boost::spirit::ascii::space_type> extendedPitch_;
        boost::spirit::qi::rule<Iterator, documentModel::AliasPitchDef(), boost::spirit::ascii::space_type> alias_;
        boost::spirit::qi::rule<Iterator, documentModel::PitchDef(), boost::spirit::ascii::space_type> pitch_;
        boost::spirit::qi::rule<Iterator, documentModel::FractionValue(), boost::spirit::ascii::space_type> fraction_;
        boost::spirit::qi::rule<Iterator, com::String(), ascii::space_type> quoted_string;
        static const std::string ALLOWED_META_ARGUMENT;
    };
}