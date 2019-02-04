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
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "error.hpp"
#include <sstream>
#include "sheet/parserSymbols.h"

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
	sheet::AliasPitch,
	(fm::String, alias)
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
	sheet::ChordEvent,
	(sheet::Event::Type, type)
	(fm::String, chordName)
	(fm::String, metaCommand)
	(sheet::Event::Args, metaArgs)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::TrackInfo,
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Track,
	(sheet::Track::TrackInfos, trackInfos)
	(sheet::Track::Voices, voices)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetInfo,
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetDef,
	(sheet::SheetDef::SheetInfos, sheetInfos)
	(sheet::SheetDef::Tracks, tracks)
	(sheet::SheetDef::Events, chords)
)


namespace sheet {
	namespace compiler {

		namespace {
			DegreeSymbols degreeSymbols_;
			PitchSymbols pitchSymbols_;
			OctaveSymbols octaveSymbols_;
			DurationSymbols durationSymbols_;
			ExpressionSymbols expressionSymbols_;
		}

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;
		
			struct ASheetParser
			{
				virtual ~ASheetParser() = default;
				ASheetParser()
				{
				}

				template<class SheetInfoRules>
				void createSheetInfoRules(SheetInfoRules &sheetInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					sheetInfo %= 
						+char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_("a-zA-Z0-9")])
						>> ";";
				}

				template<class TrackInfoRules>
				void createTrackInfoRules(TrackInfoRules &trackInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					trackInfo %= 
						+char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_("a-zA-Z0-9")])
						>> ";";
				}

				template<class TrackRules, class VoiceRules, class TrackInfoRules>
				void createTrackRules(TrackRules &track, VoiceRules &voice, TrackInfoRules &trackInfo) const
				{
					createTrackInfoRules(trackInfo);
					track %= "[" > *trackInfo > +voice > "]";
				}

				template <int EventType>
				auto noteEventAppendings() {
					using qi::lit;
					using qi::_val;
					using qi::attr;
					using boost::phoenix::at_c;
					constexpr Event::Type tieType = EventType == Event::Degree 
															   ? Event::TiedDegree
															   : Event::TiedNote;
					return 
						(durationSymbols_ | attr(Event::NoDuration))  
						>> -(
								lit("~")[at_c<0>(_val) = tieType] 
								| (lit("`")[at_c<0>(_val) = Event::Meta][at_c<3>(_val) = FM_STRING("vorschlag")])
							);
				}
			};
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SheetParser : public ASheetParser,
							      qi::grammar<Iterator, SheetDef(), ascii::space_type>
			{

				_SheetParser() : _SheetParser::base_type(start, "sheet")
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
					using boost::phoenix::at_c;

					start.name("sheet");

					event_.name("event");
					track.name("track");
					voice.name("voice");
					events.name("events");
					pitch_.name("pitch");
					degree_.name("pitch");
					degree_ %= degreeSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));

					pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
					alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
					pitchOrAlias_ %= pitch_ | alias_;
					event_ %= 
					(
						attr(Event::Note) 
						>> (pitchOrAlias_ | ("<" >> +pitchOrAlias_ >> ">")) 
						>> noteEventAppendings<Event::Note>()
					)
					|
					(
						attr(Event::Degree) 
						>> (degree_ | ("<" >> +degree_ >> ">")) 
						>> noteEventAppendings<Event::Degree>()
					)
					| 
					(
						"\\" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("expression") 
						>> expressionSymbols_
					)
					| 
					(
						"!" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("singleExpression") 
						>> expressionSymbols_
					)
					| 
					(
						"r" 
						>> attr(Event::Rest) 
						>> attr(PitchDef()) 
						>> (durationSymbols_ | attr(Event::NoDuration)))
					| 
					(
						"|" 
						>> attr(Event::EOB) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration)
					)
					| 
					(
						"/" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> +char_("a-zA-Z") >> ":" >> +(lexeme[+char_("a-zA-Z0-9")]) >> "/"
					)
					;
					events %= *event_;

					chord_ %= (attr(Event::Chord) >> lexeme[char_("a-gA-G") > *char_(ChordDefParser::ALLOWED_CHORD_SYMBOLS_REGEX)])
						| ("r" >> attr(Event::Rest) >> attr(""))
						| ("|" >> attr(Event::EOB) >> attr(""))
						| ("/" >> attr(Event::Meta) >> attr("") >> +char_("a-zA-Z") >> ":" >> +(lexeme[+char_("a-zA-Z0-9")]) >> "/")
						;
					chords_ %= *chord_;

					voice %= "{" >> events >> "}";

					createTrackRules(track, voice, trackInfo_);
					createSheetInfoRules(sheetInfo_);
					start %= *sheetInfo_ >> *track >> chords_;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);

				}
				qi::rule<Iterator, PitchDef(), ascii::space_type> degree_;
				qi::rule<Iterator, SheetDef(), ascii::space_type> start;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitch_;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitchOrAlias_;
				qi::rule<Iterator, Track(), ascii::space_type> track;
				qi::rule<Iterator, AliasPitch(), ascii::space_type> alias_;
				qi::rule<Iterator, Voice(), ascii::space_type> voice;
				qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
				qi::rule<Iterator, Event(), ascii::space_type> event_;
				qi::rule<Iterator, SheetInfo(), ascii::space_type> sheetInfo_;
				qi::rule<Iterator, SheetDef::Events(), ascii::space_type> chords_;
				qi::rule<Iterator, ChordEvent(), ascii::space_type> chord_;
				qi::rule<Iterator, TrackInfo(), ascii::space_type> trackInfo_;
			};


			void _parse(const fm::String &defStr, SheetDef &def)
			{
				using boost::spirit::ascii::space;
				typedef _SheetParser<fm::String::const_iterator> SheetParserType;
				SheetParserType g;
				phrase_parse(defStr.begin(), defStr.end(), g, space, def);
			}
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