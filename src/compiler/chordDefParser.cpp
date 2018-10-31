#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "error.hpp"

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ChordDef,
	(fm::String, name)
	(sheet::ChordDef::Intervals, intervals)
)

namespace sheet {
	namespace compiler {

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _SectionParser : qi::grammar<Iterator, ChordDef(), ascii::space_type>
			{
				typedef ChordDef::Intervals Intervals;
				_SectionParser() : _SectionParser::base_type(start, "chord def")
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;

					chordName.name("chord name");
					intervals.name("intervals");

					chordName %= char_("Xx") > *char_("a-zA-Z0-9/+#~*!?-");
					intervals %= +int_;
					start %= chordName > ':' > intervals;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);
				}
				qi::rule<Iterator, Intervals(), ascii::space_type> intervals;
				qi::rule<Iterator, fm::String(), ascii::space_type> chordName;
				qi::rule<Iterator, ChordDef(), ascii::space_type> start;
			};


			void _parse(const fm::String &defStr, ChordDef &def) 
			{
				using boost::spirit::ascii::space;
				typedef _SectionParser<fm::String::const_iterator> ChordParserType;
				ChordParserType g;
				bool r = phrase_parse(defStr.begin(), defStr.end(), g, space, def);
			}
		}


		ChordDefParser::ChordDefs ChordDefParser::parse(fm::CharType const* first, fm::CharType const* last)
		{
			
			ChordDefs result;
			ChordDefTokenizer<LexerType> chordDefTok;
			LexerType::iterator_type iter = chordDefTok.begin(first, last);
			LexerType::iterator_type end = chordDefTok.end();
			boost::spirit::lex::tokenize(first, last, chordDefTok);
			for (const auto& defStr : chordDefTok.chordDefs) {
				ChordDef def;
				_parse(defStr, def);
				result.push_back(def);
			}

			return result;
		}
	}
}