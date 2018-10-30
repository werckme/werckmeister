#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <fm/literals.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Voice,
	(sheet::Voice::Events, events)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Event,
	(sheet::Event::Pitch, pitch)
	(sheet::Event::Type, type)
	(sheet::Event::Duration, duration)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Track,
	(sheet::Track::Voices, voices)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Section,
	(fm::String, name)
	(sheet::Section::Tracks, tracks)
)

namespace sheet {
	namespace compiler {

		struct degree_ : boost::spirit::qi::symbols<char, Event::Pitch >
		{
			degree_()
			{
				add
				("I", 1)
					("II", 2)
					("III", 3)
					("IV", 4)
					("V", 5)
					("VI", 6)
					("VII", 7)
					;
			}

		} degree_;

		struct duration_ : boost::spirit::qi::symbols<char, fm::Ticks>
		{
			duration_()
			{
				using namespace fm;
				add
				("1", 1.0_N1)
					("2", 1.0_N2)
					("4", 1.0_N4)
					("8", 1.0_N8)
					("16", 1.0_N16)
					("32", 1.0_N32)
					("64", 1.0_N64)
					("128", 1.0_N128)
					("1.", 1.0_N1p)
					("2.", 1.0_N2p)
					("4.", 1.0_N4p)
					("8.", 1.0_N8p)
					("16.", 1.0_N16p)
					("32.", 1.0_N32p)
					("64.", 1.0_N64p)
					("128.", 1.0_N128p)
					;
			}

		} duration_;

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _SectionParser : qi::grammar<Iterator, Section(), ascii::space_type>
			{
				//typedef ChordDef::Intervals Intervals;
				_SectionParser() : _SectionParser::base_type(start)
				{
					using qi::int_;
					using qi::lit;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;

					event_ %= (degree_ >> attr(Event::Degree) >> (duration_ | attr(Event::NoDuration)))
							| ("r" >> attr(Event::NoPitch) >> attr(Event::Rest) >> (duration_ | attr(Event::NoDuration)))
							| ("|" >> attr(Event::NoPitch) >> attr(Event::EOB) >> attr(0));
					events %= *event_;
					voice %= "{" >> events >> "}";
					sectionName %= "section" >> *char_("a-zA-Z0-9");
					track %= "[" >> *voice >> "]";
					start %= sectionName >> +track >> "end";
				}
				qi::rule<Iterator, Section(), ascii::space_type> start;
				qi::rule<Iterator, fm::String(), ascii::space_type> sectionName;
				qi::rule<Iterator, Event(), ascii::space_type> event_;
				qi::rule<Iterator, Track(), ascii::space_type> track;
				qi::rule<Iterator, Voice(), ascii::space_type> voice;
				qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
			};


			void _parse(const fm::String &defStr, Section &def)
			{
				using boost::spirit::ascii::space;
				typedef _SectionParser<fm::String::const_iterator> SectionParserType;
				SectionParserType g;
				bool r = phrase_parse(defStr.begin(), defStr.end(), g, space, def);
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
				_parse(defStr, sec);
				result.sections.push_back(sec);
			}

			return result;
		}
	}
}