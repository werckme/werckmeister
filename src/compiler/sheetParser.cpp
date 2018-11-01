#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/vector.hpp>
#include <fm/literals.hpp>
#include "error.hpp"
#include <sstream>


BOOST_FUSION_ADAPT_STRUCT(
	sheet::Voice,
	(sheet::Voice::Events, events)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::PitchDef,
	(sheet::PitchDef::Pitch, pitch)
	(sheet::PitchDef::Octave, octave)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Event,
	(sheet::Event::Type, type)
	(sheet::Event::Pitches, pitches)
	(sheet::Event::Duration, duration)
	(fm::String, metaCommand)
	(sheet::Event::Args, metaArgs)
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

		struct degree_ : boost::spirit::qi::symbols<char, PitchDef::Pitch >
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

		struct octave_ : boost::spirit::qi::symbols<char, PitchDef::Octave >
		{
			octave_()
			{
				add
				(",,,", -3)
					(",,", -2)
					(",", -1)
					("'", 1)
					("''", 2)
					("'''", 3)
					;
			}

		} octave_;

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
					("1t", 1.0_N1t)
					("2t", 1.0_N2t)
					("4t", 1.0_N4t)
					("8t", 1.0_N8t)
					("16t", 1.0_N16t)
					("32t", 1.0_N32t)
					("64t", 1.0_N64t)
					("128t", 1.0_N128t)
					("1n5", 1.0_N1n5)
					("2n5", 1.0_N2n5)
					("4n5", 1.0_N4n5)
					("8n5", 1.0_N8n5)
					("16n5", 1.0_N16n5)
					("32n5", 1.0_N32n5)
					("64n5", 1.0_N64n5)
					("128n5", 1.0_N128n5)
					("1n7", 1.0_N1n7)
					("2n7", 1.0_N2n7)
					("4n7", 1.0_N4n7)
					("8n7", 1.0_N8n7)
					("16n7", 1.0_N16n7)
					("32n7", 1.0_N32n7)
					("64n7", 1.0_N64n7)
					("128n7", 1.0_N128n7)
					("1n9", 1.0_N1n9)
					("2n9", 1.0_N2n9)
					("4n9", 1.0_N4n9)
					("8n9", 1.0_N8n9)
					("16n9", 1.0_N16n9)
					("32n9", 1.0_N32n9)
					("64n9", 1.0_N64n9)
					("128n9", 1.0_N128n9)
					;
			}

		} duration_;

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct ASheetParser 
			{
				ASheetParser()
				{
					using qi::int_;
					using qi::lit;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;
					using qi::on_error;
					using qi::fail;

					event_.name("event");
					pitch_.name("pitch");
					track.name("track");
					voice.name("voice");
					events.name("events");

					track %= "[" > +voice > "]";

					pitch_ %= degree_ >> (octave_ | attr(PitchDef::DefaultOctave));
					event_ %= (attr(Event::Degree) >> (pitch_ | ("<" >> +pitch_ >> ">")) >> (duration_ | attr(Event::NoDuration)))
						| ("r" >> attr(Event::Rest) >> attr(PitchDef()) >> (duration_ | attr(Event::NoDuration)))
						| ("|" >> attr(Event::EOB) >> attr(PitchDef()) >> attr(Event::NoDuration))
						| ("/" >> attr(Event::Meta) >> attr(PitchDef()) >> attr(Event::NoDuration) >> +char_("a-zA-Z") >> ":" >> +(lexeme[+char_("a-zA-Z0-9")]) >> "/")
						;
					events %= *event_;
					voice %= "{" >> events >> "}";
				}
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitch_;
				qi::rule<Iterator, Track(), ascii::space_type> track;
				qi::rule<Iterator, Voice(), ascii::space_type> voice;
				qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
				qi::rule<Iterator, Event(), ascii::space_type> event_;

			};
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SectionParser : qi::grammar<Iterator, Section(), ascii::space_type>, public ASheetParser<Iterator>
			{

				_SectionParser() : _SectionParser::base_type(start, "section"), ASheetParser<Iterator>()
				{
					using qi::int_;
					using qi::lit;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;
					using qi::on_error;
					using qi::fail;

					start.name("section");
					sectionName.name("section name");

					sectionName %= "section" > *char_("a-zA-Z0-9");
					start %= sectionName > +track > "end";

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);
				}
				qi::rule<Iterator, Section(), ascii::space_type> start;
				qi::rule<Iterator, fm::String(), ascii::space_type> sectionName;
			};

			void _parse(const fm::String &defStr, Section &def)
			{
				using boost::spirit::ascii::space;
				typedef _SectionParser<fm::String::const_iterator> SectionParserType;
				SectionParserType g;
				phrase_parse(defStr.begin(), defStr.end(), g, space, def);
			}
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SheetParser : qi::grammar<Iterator, SheetDef(), ascii::space_type>, public ASheetParser<Iterator>
			{

				_SheetParser() : _SheetParser::base_type(start, "sheet"), ASheetParser<Iterator>()
				{
					using qi::int_;
					using qi::lit;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;
					using qi::on_error;
					using qi::fail;

					/*start.name("section");
					sectionName.name("section name");

					sectionName %= "section" > *char_("a-zA-Z0-9");
					start %= sectionName > +track > "end";

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);*/
					//on_error<fail>(start, onError);
				}
				qi::rule<Iterator, SheetDef(), ascii::space_type> start;
			};


			void _parse(const fm::String &defStr, SheetDef &def)
			{
				using boost::spirit::ascii::space;
				typedef _SheetParser<fm::String::const_iterator> SheetParserType;
				SheetParserType g;
				phrase_parse(defStr.begin(), defStr.end(), g, space, def);
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

		SheetDef SheetDefParser::parse(fm::CharType const* first, fm::CharType const* last)
		{

			SheetDef result;
			SheetDefTokenizer<LexerType> sheetDefTok;
			LexerType::iterator_type iter = sheetDefTok.begin(first, last);
			LexerType::iterator_type end = sheetDefTok.end();
			boost::spirit::lex::tokenize(first, last, sheetDefTok);
			_parse(sheetDefTok.tracks.str(), result);
			return result;
		}
	}
}