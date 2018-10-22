#include "parser.h"
#include "lexer.h"

//BOOST_FUSION_ADAPT_STRUCT(
//	sheet::ChordDef,
//	(fm::String, name)
//	(sheet::ChordDef::Intervals, intervals)
//)

namespace sheet {
	namespace compiler {

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _SectionParser : qi::grammar<Iterator, ChordDef(), ascii::space_type>
			{
				//typedef ChordDef::Intervals Intervals;
			/*	_SectionParser() : _SectionParser::base_type(start)
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;

					chordName %= char_("Xx") >> *char_("a-zA-Z0-9/+#~*!?-");
					intervals %= +int_;
					start %= chordName >> ':' >> intervals;
				}*/
				/*qi::rule<Iterator, Intervals(), ascii::space_type> intervals;
				qi::rule<Iterator, fm::String(), ascii::space_type> chordName;
				qi::rule<Iterator, ChordDef(), ascii::space_type> start;*/
			};


			void _parse(const fm::String &defStr, Section &def)
			{
				//using boost::spirit::ascii::space;
				//typedef _SectionParser<fm::String::const_iterator> SectionParserType;
				//SectionParserType g;
				//bool r = phrase_parse(defStr.begin(), defStr.end(), g, space, def);
			}
		}


		StyleDef StyleDefParser::parse(fm::CharType const* first, fm::CharType const* last)
		{

			StyleDef result;
			StyleDefTokenizer<LexerType> styleDefTok;
			LexerType::iterator_type iter = styleDefTok.begin(first, last);
			LexerType::iterator_type end = styleDefTok.end();
			boost::spirit::lex::tokenize(first, last, styleDefTok);
			for (const auto& defStr : styleDefTok.sections) {
				Section sec;
				//_parse(defStr, def);
				result.sections.push_back(sec);
			}

			return result;
		}
	}
}