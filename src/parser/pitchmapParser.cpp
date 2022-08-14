#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "parserSymbols.h"
#include "error.hpp"

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::PitchDef,
	(documentModel::PitchDef::Pitch, pitch)(documentModel::PitchDef::Octave, octave))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Pitchmap,
	(com::String, name)(documentModel::PitchDef, pitch))

namespace parser
{

	namespace
	{
		PitchSymbols pitchSymbols_;
		OctaveSymbols octaveSymbols_;
	}

	namespace
	{
		namespace qi = boost::spirit::qi;
		namespace ascii = boost::spirit::ascii;
		using namespace documentModel;
		template <typename Iterator>
		struct _PitchmapParser : qi::grammar<Iterator, Pitchmap(), ascii::space_type>
		{
			typedef ChordDef::DegreeDefs DegreeDefs;
			_PitchmapParser() : _PitchmapParser::base_type(start, "chord def")
			{
				using ascii::char_;
				using qi::attr;
				using qi::double_;
				using qi::fail;
				using qi::int_;
				using qi::lexeme;
				using qi::lit;
				using qi::on_error;

				alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];

				pitch_.name("pitch");
				pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));

				start %= alias_ > ':' > pitch_;

				auto onError = boost::bind(&compiler::handler::errorHandler<Iterator>, _1);
				on_error<fail>(start, onError);
			}
			qi::rule<Iterator, Pitchmap(), ascii::space_type> start;
			qi::rule<Iterator, PitchDef(), ascii::space_type> pitch_;
			qi::rule<Iterator, com::String(), ascii::space_type> alias_;
		};

		void _parse(const com::String &defStr, Pitchmap &def)
		{
			using boost::spirit::ascii::space;
			typedef _PitchmapParser<com::String::const_iterator> PitchmapParserType;
			PitchmapParserType g;
			phrase_parse(defStr.begin(), defStr.end(), g, space, def);
		}
	}

	PitchmapParser::PitchmapDefs PitchmapParser::parse(com::CharType const *first, com::CharType const *last)
	{

		PitchmapDefs result;
		PitchmapTokenizer<LexerType> pitchDefTok;
		LexerType::iterator_type iter = pitchDefTok.begin(first, last);
		LexerType::iterator_type end = pitchDefTok.end();
		boost::spirit::lex::tokenize(first, last, pitchDefTok);
		for (const auto &defStr : pitchDefTok.pitchdefs)
		{
			Pitchmap def;
			_parse(defStr, def);
			result.push_back(def);
		}
		return result;
	}
}